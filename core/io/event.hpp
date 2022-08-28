#ifndef _ISM_EVENT_HPP_
#define _ISM_EVENT_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Event;

	template <class> class EventClass;

	class EventHandler;

	class DummyHandler;

	template <class> class EventDelegate;

	class EventBus;

	using EventID = size_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event
	class ISM_API Event : public Object
	{
		OBJECT_CLASS(Event, Object);

	protected:
		Event() noexcept = default;

	public:
		enum : EventID { ID = static_cast<EventID>(-1) };

		virtual ~Event() noexcept override = default;

		virtual EventID get_event_id() const = 0;

		operator EventID() const { return get_event_id(); }

		bool operator==(EventID const id) const noexcept { return id == get_event_id(); }

		bool operator!=(EventID const id) const noexcept { return id != get_event_id(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event class
#define EVENT_CLASS(m_class, m_inherits)										\
private:																		\
	static_assert(std::is_base_of_v<ism::Event, m_inherits>);					\
																				\
	OBJECT_CLASS(m_class, m_inherits);											\
																				\
public:																			\
	enum : ism::EventID { ID = m_class::get_class_static().hash_code() };		\
																				\
	virtual ism::EventID get_event_id() const override { return m_class::ID; }	\
																				\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// handler
	class ISM_API EventHandler : public Object
	{
		OBJECT_CLASS(EventHandler, Object);

		friend struct Less<EventHandler *>;

		EventBus * const m_event_bus;
		
		i32 const m_dispatch_order;

	public:
		virtual ~EventHandler() noexcept override { unsubscribe(); }

		auto get_event_bus() const noexcept -> EventBus * { return m_event_bus; }

	protected:
		friend class EventBus;

		explicit EventHandler(EventBus * bus = nullptr) noexcept;

		template <class Event0, class ... Events
		> void subscribe() noexcept;

		template <class ... Events
		> void unsubscribe() noexcept;

		virtual void handle_event(Event const &) = 0; // <- HANDLE EVENT
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dummy handler
	class ISM_API DummyHandler final : public EventHandler
	{
		OBJECT_CLASS(DummyHandler, EventHandler);

		std::function<void(Event const &)> m_callback{};

	public:
		template <class Fn
		> DummyHandler(EventBus * bus, Fn && fn) noexcept : EventHandler{ bus }, m_callback{ FWD(fn) } {}

		auto get_callback() const noexcept -> auto const & { return m_callback; }

		template <class Fn
		> void set_callback(Fn && fn) noexcept { m_callback = FWD(fn); }

		using EventHandler::subscribe;

		using EventHandler::unsubscribe;

	protected:
		void handle_event(Event const & event) final { m_callback(event); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// delegate base
	template <> class ISM_API EventDelegate<Event> : public EventHandler
	{
		OBJECT_CLASS(EventDelegate<Event>, EventHandler);

	public:
		enum : EventID { ID = Event::ID };

		using Callback = typename void;

		virtual ~EventDelegate() noexcept override = default;

	protected:
		explicit EventDelegate(EventBus * bus) noexcept : EventHandler{ bus } {}

		virtual void handle_event(Event const & event) override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// delegate
	template <class Ev> class EventDelegate final : public EventDelegate<Event>
	{
		Vector<std::function<void(Ev const &)>> m_callbacks{};

	public:
		enum : EventID { ID = Ev::ID };

		using Callback = typename decltype(m_callbacks)::value_type;

		explicit EventDelegate(EventBus * bus) noexcept : EventDelegate<Event>{ bus } { this->subscribe<Ev>(); }

		auto operator[](size_t i) noexcept -> Callback & { return m_callbacks[i]; }

		auto operator[](size_t i) const noexcept -> Callback const & { return m_callbacks[i]; }

		void clear() noexcept { m_callbacks.clear(); }

		void erase(size_t i, size_t n = 1) { m_callbacks.erase(m_callbacks.begin() + i, m_callbacks.begin() + i + n); }

		void reserve(size_t count) { m_callbacks.reserve(count); }
		
		template <class Fn
		> auto add(Fn && fn) noexcept -> Callback & { return m_callbacks.emplace_back(VALIDATE(Callback{ FWD(fn) })); }

		template <class Fn
		> auto insert(size_t i, Fn && fn) noexcept -> Callback & { return m_callbacks.emplace(m_callbacks.begin() + i, VALIDATE(Callback{ FWD(fn) })); }

		template <class Fn
		> auto operator+=(Fn && fn) noexcept -> EventDelegate & { return this->add(FWD(fn)), (*this); }

	protected:
		void handle_event(Event const & ev) final
		{
			ASSERT((EventID)ev == ID);

			for (Callback const & callback : m_callbacks)
			{
				callback(static_cast<Ev const &>(ev));
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct Less<EventHandler *>
	{
		bool operator()(EventHandler * a, EventHandler * b) const noexcept
		{
			return (a != b) && ((a && b) ? (a->m_dispatch_order < b->m_dispatch_order) : (a < b));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// bus
	class ISM_API EventBus : public Object
	{
		OBJECT_CLASS(EventBus, Object);

		static EventBus * __singleton;

		FlatMap<EventID, FlatSet<EventHandler *>> m_event_handlers{};
		FlatMap<EventID, Ref<EventDelegate<Event>>> m_event_delegates{};
		Vector<Ref<DummyHandler>> m_dummy_handlers{};

	protected:
		friend class EventHandler;
		i32 m_next_index{};

	public:
		EventBus() noexcept { __singleton = this; }
		virtual ~EventBus() noexcept override = default;
		FORCE_INLINE static EventBus * get_singleton() noexcept { return __singleton; }

	public:
		/* DISPATCH */

		void fire_event(Event const & value) noexcept
		{
			if (auto const group{ m_event_handlers.find((EventID)value) })
			{
				for (EventHandler * const handler : (*group->second))
				{
					VALIDATE(handler)->handle_event(value);
				}
			}
		}

		template <class Ev> bool add_event_handler(EventHandler * value) noexcept
		{
			return value && m_event_handlers[Ev::ID].insert(value).second;
		}

		template <class Ev> void remove_event_handler(EventHandler * value) noexcept
		{
			if (auto const group{ m_event_handlers.find(Ev::ID) })
			{
				if (auto const it{ group->second->find(value) }; it != group->second->end())
				{
					group->second->erase(it);
				}
			}
		}

		void remove_event_handler(EventHandler * value) noexcept
		{
			m_event_handlers.for_each([&](auto, FlatSet<EventHandler *> & group) noexcept
			{
				if (auto const it{ group.find(value) }; it != group.end())
				{
					group.erase(it);
				}
			});
		}

	public:
		/* DUMMIES */

		NODISCARD auto get_all_dummies() const noexcept -> auto const &
		{
			return m_dummy_handlers;
		}

		template <class ... Events, class Fn
		> auto & add_dummy_handler(Fn && fn) noexcept
		{
			Ref<DummyHandler> dummy{ memnew(DummyHandler(this, FWD(fn))) };

			if constexpr (0 < sizeof...(Events)) { dummy->subscribe<Events...>(); }

			return m_dummy_handlers.emplace_back(dummy);
		}

		auto remove_dummy_handler(Ref<DummyHandler> const & value) noexcept
		{
			if (auto const it{ m_dummy_handlers.find(value) }; it != m_dummy_handlers.end())
			{
				return m_dummy_handlers.erase(it);
			}
			else
			{
				return it;
			}
		}

		void remove_all_dummy_handlers() noexcept
		{
			while (!m_dummy_handlers.empty()) { m_dummy_handlers.pop_back(); }
		}

	public:
		/* DELEGATES */

		NODISCARD auto get_all_delegates() const noexcept -> auto const &
		{
			return m_event_delegates;
		}

		template <class Ev
		> auto get_delegate() noexcept -> EventDelegate<Ev> &
		{
			return **((Ref<EventDelegate<Ev>> &)m_event_delegates.find_or_add_fn(Ev::ID, [&
			]() noexcept { return memnew(EventDelegate<Ev>(this)); }));
		}

		template <class ... Events
		> void remove_delegates() noexcept
		{
			if constexpr (0 < sizeof...(Events))
			{
				mpl::for_types<Events...>([&](auto tag) noexcept
				{
					using T = TAG_TYPE(tag);

					if (auto const it{ m_event_delegates.find(hashof_v<T>) })
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

	inline EventHandler::EventHandler(EventBus * bus) noexcept
		: m_event_bus{ bus ? bus : VALIDATE(EventBus::get_singleton()) }
		, m_dispatch_order{ ++m_event_bus->m_next_index }
	{
	}

	template <class Event0, class ... Events
	> void EventHandler::subscribe() noexcept
	{
		ASSERT(m_event_bus);

		mpl::for_types<Event0, Events...>([&](auto tag) noexcept
		{
			m_event_bus->add_event_handler<TAG_TYPE(tag)>(this);
		});
	}

	template <class ... Events
	> void EventHandler::unsubscribe() noexcept
	{
		ASSERT(m_event_bus);

		if constexpr (0 < sizeof...(Events))
		{
			mpl::for_types<Events...>([&](auto tag) noexcept
			{
				m_event_bus->remove_event_handler<TAG_TYPE(tag)>(this);
			});
		}
		else
		{
			m_event_bus->remove_event_handler(this); // remove from all events
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EVENT_HPP_
