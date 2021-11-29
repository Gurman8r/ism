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
		explicit Event(EventID const event_id) noexcept : m_event_id{ event_id } {}

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

		friend struct Less<EventHandler *>;

		EventBus * const m_bus;
		int32_t const m_order;

	public:
		virtual ~EventHandler() noexcept { unsubscribe(); }

		NODISCARD auto get_bus() const noexcept -> EventBus * { return m_bus; }

	protected:
		friend class EventBus;

		explicit EventHandler(EventBus * bus = nullptr) noexcept;

		virtual void handle_event(Event const &) = 0;

		template <class ... Evs> void subscribe() noexcept
		{
			mpl::for_types<Evs...>([&](auto tag)
			{
				m_bus->add_handler<TAG_TYPE(tag)>(this);
			});
		}

		template <class ... Evs> void unsubscribe() noexcept
		{
			if constexpr (0 < sizeof...(Evs))
			{
				mpl::for_types<Evs...>([&](auto tag)
				{
					m_bus->remove_handler<TAG_TYPE(tag)>(this);
				});
			}
			else
			{
				m_bus->remove_handler(this); // remove from all events
			}
		}
	};

	template <> struct Less<EventHandler *>
	{
		NODISCARD bool operator()(EventHandler * a, EventHandler * b) const noexcept
		{
			if (a == b) { return false; }

			else if (!a || !b) { return CMP((intptr_t)a, (intptr_t)b); }
			
			else { return util::compare(a->m_order, b->m_order) < 0; }
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dummy handler
	class ISM_API DummyHandler final : public EventHandler
	{
		OBJECT_COMMON(DummyHandler, EventHandler);

		EventCallback m_callback{};

	public:
		~DummyHandler() noexcept final = default;

		template <class Fn
		> DummyHandler(EventBus * bus, Fn && fn) noexcept : EventHandler{ bus }, m_callback{ FWD(fn) } {}

		void handle_event(Event const & ev) final { VALIDATE(m_callback)(ev); }

		NODISCARD auto get_callback() const noexcept -> EventCallback const & { return m_callback; }

		template <class Fn> void set_callback(Fn && fn) noexcept { m_callback = FWD(fn); }

		using EventHandler::subscribe;

		using EventHandler::unsubscribe;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event delegate base
	template <> class ISM_API EventDelegate<void> : public EventHandler
	{
		OBJECT_COMMON(EventDelegate<void>, EventHandler);

	protected:
		explicit EventDelegate(EventBus * bus) noexcept : EventHandler{ bus } {}

	public:
		virtual ~EventDelegate() noexcept override = default;

		virtual void handle_event(Event const & ev) override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event delegate
	template <class Ev> class EventDelegate final : public EventDelegate<void>
	{
		Vector<std::function<void(Ev const &)>> m_data{};

	public:
		using base_type = EventDelegate<void>;

		using self_type = EventDelegate<Ev>;

		using storage_type = decltype(m_data);

		using value_type = typename storage_type::value_type;

		explicit EventDelegate(EventBus * bus) noexcept : base_type{ bus } { this->subscribe<Ev>(); }

		template <class Ev> void dispatch(Ev && value) noexcept { this->handle_event(FWD(value)); }

		void clear() noexcept { m_data.clear(); }

		void erase(size_t i, size_t n = 1) noexcept { m_data.erase(m_data.begin() + i, m_data.begin() + i + n); }

		void reserve(size_t count) noexcept { m_data.reserve(count); }

		NODISCARD auto operator[](size_t i) noexcept -> value_type & { return m_data[i]; }

		NODISCARD auto operator[](size_t i) const noexcept -> value_type const & { return m_data[i]; }
		
		template <class Fn
		> auto add(Fn && fn) noexcept -> value_type & { return m_data.emplace_back(FWD(fn)); }

		template <class Fn
		> auto insert(size_t i, Fn && fn) noexcept -> value_type & { return m_data.emplace(m_data.begin() + i, FWD(fn)); }

		template <class Fn
		> auto operator+=(Fn && fn) noexcept -> self_type & { return this->add(FWD(fn)), (*this); }

	protected:
		void handle_event(Event const & ev) final
		{
			for (value_type const & callback : m_data)
			{
				callback((Ev const &)ev);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event bus
	class ISM_API EventBus : public Object
	{
		OBJECT_COMMON(EventBus, Object);

		static EventBus * singleton;

		int32_t m_next_id{};
		FlatMap<EventID, FlatSet<EventHandler *>> m_handlers{};
		FlatMap<EventID, EventDelegate<void> *> m_delegates{};
		Vector<std::shared_ptr<DummyHandler>> m_dummies{};

	protected:
		friend class EventHandler;

		NODISCARD auto next_id() noexcept { return ++m_next_id; }

	public:
		EventBus() noexcept { singleton = this; }

		virtual ~EventBus() noexcept override { remove_delegates(); }

		NODISCARD static EventBus * get_singleton() noexcept { return singleton; }

		template <class Ev = Event
		> void fire_event(Ev && value) noexcept
		{
			if (auto const group{ m_handlers.find((EventID)value) })
			{
				for (auto const e : (*group->second))
				{
					VALIDATE(e)->handle_event(value);
				}
			}
		}

	public:
		/* LISTENERS */

		template <class Ev> bool add_handler(EventHandler * value) noexcept
		{
			return m_handlers[Ev::ID].insert(value).second;
		}

		template <class Ev> void remove_handler(EventHandler * value) noexcept
		{
			if (auto const group{ m_handlers.find(Ev::ID) })
			{
				if (auto const e{ group->second->find(value) }; e != group->second->end())
				{
					group->second->erase(e);
				}
			}
		}

		void remove_handler(EventHandler * value) noexcept
		{
			m_handlers.for_each([&](auto, FlatSet<EventHandler *> & group) noexcept
			{
				if (auto const e{ group.find(value) }; e != group.end())
				{
					group.erase(e);
				}
			});
		}

	public:
		/* DUMMIES */

		NODISCARD auto const & get_all_dummies() const noexcept { return m_dummies; }

		template <class ... Evs, class Fn
		> auto & add_dummy(Fn && fn) noexcept
		{
			auto temp{ std::make_shared<DummyHandler>(this, FWD(fn)) };

			if constexpr (0 < sizeof...(Evs)) { temp->subscribe<Evs...>(); }

			return m_dummies.emplace_back(std::move(temp));
		}

		auto remove_dummy(std::shared_ptr<DummyHandler> const & value) noexcept
		{
			if (auto it{ std::find(m_dummies.begin(), m_dummies.end(), value) }; it == m_dummies.end())
			{
				return it;
			}
			else
			{
				return m_dummies.erase(it);
			}
		}

		void remove_all_dummies() noexcept
		{
			while (!m_dummies.empty()) { m_dummies.pop_back(); }
		}

	public:
		/* DELEGATES */

		NODISCARD auto const & get_all_delegates() const noexcept { return m_delegates; }

		template <class Ev
		> NODISCARD auto get_delegate() noexcept -> EventDelegate<Ev> &
		{
			return *(EventDelegate<Ev> *)(m_delegates.find_or_add_fn(hash_v<Ev>, [&]() noexcept
			{
				return memnew(EventDelegate<Ev>(this));
			}));
		}

		template <class ... Evs
		> void remove_delegates() noexcept
		{
			if constexpr (0 < sizeof...(Evs))
			{
				mpl::for_types<Evs...>([&](auto tag) noexcept
				{
					using T = TAG_TYPE(tag);

					if (auto const it{ m_delegates.find(hash_v<T>) })
					{
						memdelete((EventDelegate<T> *)(*it->second));

						m_delegates.erase(it->first);
					}
				});
			}
			else
			{
				m_delegates.for_each([&](auto, auto ptr) noexcept { memdelete(ptr); });

				m_delegates.clear();
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EVENT_HPP_
