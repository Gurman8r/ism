#ifndef _ISM_EVENT_HPP_
#define _ISM_EVENT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Event;

	template <class> class EventClass;

	class EventHandler;

	class DummyHandler;

	template <class> class EventDelegate;

	class EventBus;

	ALIAS(EventID) hash_t;

	ALIAS(EventCallback) std::function<void(Event const &)>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event
	class ISM_API Event : public Resource
	{
		OBJECT_COMMON(Event, Resource);

		EventID m_event_id;

	protected:
		explicit Event(EventID const id) noexcept : m_event_id{ id } {}

	public:
		virtual ~Event() noexcept override = default;

		COPYABLE_MOVABLE(Event);

		NODISCARD EventID get_event_id() const { return m_event_id; }

		NODISCARD operator EventID() const { return m_event_id; }

		NODISCARD bool operator==(EventID const id) const noexcept { return id == m_event_id; }

		NODISCARD bool operator!=(EventID const id) const noexcept { return id != m_event_id; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event class helper
	template <class Derived
	> class EventClass : public Event
	{
	protected:
		explicit EventClass() noexcept : Event{ ID } {}

	public:
		enum : EventID { ID = hash_v<Derived> };

		virtual ~EventClass() noexcept override = default;

		COPYABLE_MOVABLE(EventClass);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event handler
	class ISM_API EventHandler : public Object
	{
		OBJECT_COMMON(EventHandler, Object);

		EventBus * const m_event_bus;
		
		int32_t const m_handler_id;

	protected:
		friend class EventBus;

		explicit EventHandler(EventBus * bus = nullptr) noexcept;

		virtual void handle_event(Event const &) = 0;

	public:
		virtual ~EventHandler() noexcept override { unsubscribe(); }

		NODISCARD auto get_event_bus() const noexcept -> EventBus * { return m_event_bus; }

		NODISCARD auto get_handler_id() const noexcept -> int32_t { return m_handler_id; }

	public:
		template <class Ev0, class ... Evs
		> void subscribe() noexcept
		{
			VERIFY(m_event_bus);

			mpl::for_types<Ev0, Evs...>([&](auto tag) noexcept
			{
				m_event_bus->add_event_handler<TAG_TYPE(tag)>(this);
			});
		}

		template <class ... Evs
		> void unsubscribe() noexcept
		{
			VERIFY(m_event_bus);

			if constexpr (0 < sizeof...(Evs))
			{
				mpl::for_types<Evs...>([&](auto tag) noexcept
				{
					m_event_bus->remove_event_handler<TAG_TYPE(tag)>(this);
				});
			}
			else
			{
				m_event_bus->remove_event_handler(this); // remove from all events
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// handler comparator
	template <> struct Less<EventHandler *>
	{
		NODISCARD bool operator()(EventHandler * a, EventHandler * b) const noexcept
		{
			if (a == b) { return false; }

			else if (!a || !b) { return CMP((intptr_t)a, (intptr_t)b); }
			
			else { return util::compare(a->get_handler_id(), b->get_handler_id()) < 0; }
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dummy handler
	class ISM_API DummyHandler final : public EventHandler
	{
		OBJECT_COMMON(DummyHandler, EventHandler);

		EventCallback m_callback{};

	public:
		template <class Fn
		> DummyHandler(EventBus * bus, Fn && fn) noexcept : EventHandler{ bus }, m_callback{ FWD(fn) } {}

		void handle_event(Event const & ev) final { VALIDATE(m_callback)(ev); }

		NODISCARD auto get_callback() const noexcept -> EventCallback const & { return m_callback; }

		template <class Fn> void set_callback(Fn && fn) noexcept { m_callback = FWD(fn); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event delegate base
	template <> class ISM_API EventDelegate<void> : public EventHandler
	{
		OBJECT_COMMON(EventDelegate<void>, EventHandler);

	protected:
		explicit EventDelegate(EventBus * bus) noexcept : EventHandler{ bus } {}

	public:
		enum : EventID { ID = static_cast<EventID>(-1) };

		using Callback = typename void;

		virtual ~EventDelegate() noexcept override = default;

		virtual void handle_event(Event const & ev) override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event delegate
	template <class Ev> class EventDelegate final : public EventDelegate<void>
	{
		Vector<std::function<void(Ev const &)>> m_callbacks{};

	public:
		enum : EventID { ID = Ev::ID };

		using Callback = typename decltype(m_callbacks)::value_type;

		explicit EventDelegate(EventBus * bus) noexcept : EventDelegate<void>{ bus } { this->subscribe<Ev>(); }

		template <class Ev> void dispatch(Ev && value) noexcept { this->handle_event(FWD(value)); }

		NODISCARD auto operator[](size_t i) noexcept -> Callback & { return m_callbacks[i]; }

		NODISCARD auto operator[](size_t i) const noexcept -> Callback const & { return m_callbacks[i]; }

		void clear() noexcept { m_callbacks.clear(); }

		void erase(size_t i, size_t n = 1) noexcept { m_callbacks.erase(m_callbacks.begin() + i, m_callbacks.begin() + i + n); }

		void reserve(size_t count) noexcept { m_callbacks.reserve(count); }
		
		template <class Fn
		> auto add(Fn && fn) noexcept -> Callback & { return m_callbacks.emplace_back(VALIDATE(Callback{ FWD(fn) })); }

		template <class Fn
		> auto insert(size_t i, Fn && fn) noexcept -> Callback & { return m_callbacks.emplace(m_callbacks.begin() + i, VALIDATE(Callback{ FWD(fn) })); }

		template <class Fn
		> auto operator+=(Fn && fn) noexcept -> EventDelegate & { return this->add(FWD(fn)), (*this); }

	protected:
		void handle_event(Event const & ev) final
		{
			VERIFY((EventID)ev == ID);

			for (Callback const & callback : m_callbacks)
			{
				callback(static_cast<Ev const &>(ev));
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event bus
	class ISM_API EventBus : public Object
	{
		OBJECT_COMMON(EventBus, Object);

		static EventBus * singleton;

		int32_t m_next_handler_id{};
		FlatMap<EventID, FlatSet<EventHandler *>> m_event_handlers{};
		FlatMap<EventID, Ref<EventDelegate<void>>> m_event_delegates{};
		Vector<Ref<DummyHandler>> m_dummy_handlers{};

	protected:
		friend class EventHandler;

		NODISCARD auto next_handler_id() noexcept { return ++m_next_handler_id; }

	public:
		EventBus() noexcept { singleton = this; }

		virtual ~EventBus() noexcept override = default;

		NODISCARD static EventBus * get_singleton() noexcept { return singleton; }

	public:
		/* DISPATCH API */

		void fire_event(Event const & value) noexcept
		{
			if (auto const group{ m_event_handlers.find((EventID)value) })
			{
				for (auto const e : (*group->second))
				{
					VALIDATE(e)->handle_event(value);
				}
			}
		}

	public:
		/* HANDLER API */

		template <class Ev> bool add_event_handler(EventHandler * value) noexcept
		{
			return m_event_handlers[Ev::ID].insert(value).second;
		}

		template <class Ev> void remove_event_handler(EventHandler * value) noexcept
		{
			if (auto const group{ m_event_handlers.find(Ev::ID) })
			{
				if (auto const e{ group->second->find(value) }; e != group->second->end())
				{
					group->second->erase(e);
				}
			}
		}

		void remove_event_handler(EventHandler * value) noexcept
		{
			m_event_handlers.for_each([&](auto, FlatSet<EventHandler *> & group) noexcept
			{
				if (auto const e{ group.find(value) }; e != group.end())
				{
					group.erase(e);
				}
			});
		}

	public:
		/* DUMMY API */

		NODISCARD auto const & get_all_dummy_handlers() const noexcept { return m_dummy_handlers; }

		template <class ... Evs, class Fn
		> auto & add_dummy_handler(Fn && fn) noexcept
		{
			Ref<DummyHandler> dummy{ memnew(DummyHandler(this, FWD(fn))) };

			if constexpr (0 < sizeof...(Evs)) { dummy->subscribe<Evs...>(); }

			return m_dummy_handlers.emplace_back(dummy);
		}

		auto remove_dummy_handler(Ref<DummyHandler> const & value) noexcept
		{
			if (auto it{ std::find(m_dummy_handlers.begin(), m_dummy_handlers.end(), value) }; it == m_dummy_handlers.end())
			{
				return it;
			}
			else
			{
				return m_dummy_handlers.erase(it);
			}
		}

		void remove_all_dummy_handlers() noexcept
		{
			while (!m_dummy_handlers.empty()) { m_dummy_handlers.pop_back(); }
		}

	public:
		/* DELEGATE API */

		NODISCARD auto const & get_all_event_delegates() const noexcept { return m_event_delegates; }

		template <class Ev
		> NODISCARD auto get_event_delegate() noexcept -> EventDelegate<Ev> &
		{
			return **((Ref<EventDelegate<Ev>> &)m_event_delegates.find_or_add_fn(Ev::ID, [&]() noexcept {
				return memnew(EventDelegate<Ev>(this));
			}));
		}

		template <class ... Evs
		> void remove_event_delegates() noexcept
		{
			if constexpr (0 < sizeof...(Evs))
			{
				mpl::for_types<Evs...>([&](auto tag) noexcept
				{
					using T = TAG_TYPE(tag);

					if (auto const it{ m_event_delegates.find(hash_v<T>) })
					{
						m_event_delegates.erase(it);
					}
				});
			}
			else
			{
				m_event_delegates.clear();
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event handler constructor
	inline EventHandler::EventHandler(EventBus * bus) noexcept
		: m_event_bus{ bus ? bus : SINGLETON(EventBus) }
		, m_handler_id{ VALIDATE(get_event_bus())->next_handler_id() }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EVENT_HPP_
