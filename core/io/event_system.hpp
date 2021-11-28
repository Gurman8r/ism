#ifndef _ISM_EVENT_SYSTEM_HPP_
#define _ISM_EVENT_SYSTEM_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(EventID) hash_t;

	class Event;

	class EventListener;

	class DummyListener;

	template <class> class EventDelegate;

	class EventSystem;

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

	// event helper
	template <class Derived
	> class EventHelper : public Event
	{
	protected:
		explicit EventHelper() noexcept : Event{ ID } {}

	public:
		enum : EventID { ID = hash_v<Derived> };

		virtual ~EventHelper() noexcept override = default;

		COPYABLE_MOVABLE(EventHelper);
	};

#define EVENT_CLASS(m_class) \
	class ISM_API m_class : public ism::EventHelper<m_class>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event listener
	class EventListener
	{
	private:
		EventSystem * const m_bus;
		
		int32_t m_index;
		
		explicit EventListener(EventSystem * bus, int32_t i) noexcept : m_bus{ bus }, m_index{ i } {}

	public:
		virtual ~EventListener() noexcept { unsubscribe(); }

		NODISCARD auto get_bus() const noexcept { return m_bus; }

		NODISCARD auto get_bus_index() const noexcept { return m_index; }

		NODISCARD auto get_bus_order(EventListener const & other) const noexcept {
			return (this != std::addressof(other)) ? CMP(m_index, other.m_index) : 0;
		}

	protected:
		friend class EventSystem;

		template <class Bus> EventListener(Bus * bus) noexcept : EventListener{ bus, bus->next_id() } {}

		virtual void on_event(Event const &) = 0;

		template <class ... Evs> void subscribe() noexcept
		{
			VERIFY(m_bus);

			mpl::for_types<Evs...>([&](auto tag)
			{
				m_bus->add_listener<TAG_TYPE(tag)>(this);
			});
		}

		template <class ... Evs> void unsubscribe() noexcept
		{
			VERIFY(m_bus);

			if constexpr (0 < sizeof...(Evs))
			{
				mpl::for_types<Evs...>([&](auto tag)
				{
					m_bus->remove_listener<TAG_TYPE(tag)>(this);
				});
			}
			else
			{
				m_bus->remove_listener(this); // remove from all events
			}
		}
	};

	template <> struct Less<EventListener *>
	{
		NODISCARD bool operator()(EventListener * a, EventListener * b) const noexcept
		{
			if (!a || !b) { return CMP((intptr_t)a, (intptr_t)b); }
			
			else { return a->get_bus_order(*b) < 0; }
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dummy listener
	class DummyListener : public EventListener
	{
		EventCallback m_callback{};

	public:
		virtual ~DummyListener() noexcept override = default;

		template <class ... Args
		> DummyListener(EventSystem * bus, Args && ... args) noexcept : EventListener{ bus }
		{
			if constexpr (0 < sizeof...(Args)) { set_callback(FWD(args)...); }
		}

		virtual void on_event(Event const & ev) override { VALIDATE(m_callback)(ev); }

		NODISCARD EventCallback get_callback() const noexcept { return m_callback; }

		template <class Fn, class ... Args
		> void set_callback(Fn && fn, Args && ... args) { m_callback = std::bind(FWD(fn), std::placeholders::_1, FWD(args)...); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event delegate base
	template <> class EventDelegate<void> : public EventListener
	{
	protected:
		explicit EventDelegate(EventSystem * bus) noexcept : EventListener{ bus } {}

	public:
		virtual ~EventDelegate() noexcept override = default;

		virtual void on_event(Event const & ev) override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event delegate
	template <class Ev> class EventDelegate : public EventDelegate<void>
	{
		Vector<std::function<void(Ev const &)>> m_data{};

	public:
		using base_type = EventDelegate<void>;

		using storage_type = decltype(m_data);

		using value_type = typename storage_type::value_type;

		explicit EventDelegate(EventSystem * bus) noexcept : base_type{ bus } { this->subscribe<Ev>(); }

		template <class Ev> void dispatch(Ev && value) noexcept { this->on_event(FWD(value)); }

	public:
		void clear() noexcept { m_data.clear(); }

		void erase(size_t i, size_t n = 1) noexcept { m_data.erase(m_data.begin() + i, m_data.begin() + i + n); }

		void reserve(size_t count) noexcept { m_data.reserve(count); }

		NODISCARD auto at(size_t i) noexcept -> value_type & { return m_data[i]; }

		NODISCARD auto at(size_t i) const noexcept -> value_type const & { return m_data[i]; }

		NODISCARD auto operator[](size_t i) noexcept -> value_type & { return m_data[i]; }

		NODISCARD auto operator[](size_t i) const noexcept -> value_type const & { return m_data[i]; }

	public:
		auto insert(size_t i, value_type const & fn) -> value_type & { return m_data.emplace(m_data.begin() + i, fn); }

		auto insert(size_t i, value_type && fn) noexcept -> value_type & { return m_data.emplace(m_data.begin() + i, std::move(fn)); }

		template <class Fn
		> auto insert(size_t i, Fn && fn) noexcept -> value_type & { return m_data.emplace(m_data.begin() + i, FWD(fn)); }

	public:
		auto add(value_type const & fn) -> value_type & { return m_data.emplace_back(fn); }

		auto add(value_type && fn) noexcept -> value_type & { return m_data.emplace_back(std::move(fn)); }

		template <class Fn
		> auto add(Fn && fn) noexcept -> value_type & { return m_data.emplace_back(FWD(fn)); }

	public:
		auto operator+=(value_type const & fn) -> EventDelegate & { return this->add(fn), (*this); }

		auto operator+=(value_type && fn) noexcept -> EventDelegate & { return this->add(std::move(fn)), (*this); }

		template <class Fn
		> auto operator+=(Fn && fn) noexcept -> EventDelegate & { return this->add(FWD(fn)), (*this); }

	protected:
		virtual void on_event(Event const & ev) override
		{
			for (value_type const & callback : m_data)
			{
				callback((Ev const &)ev);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event system
	class ISM_API EventSystem : public Object
	{
		OBJECT_COMMON(EventSystem, Object);

		int32_t m_next_id{};
		FlatMap<EventID, FlatSet<EventListener *>> m_listeners{};
		FlatMap<EventID, EventDelegate<void> *> m_delegates{};
		Vector<std::shared_ptr<DummyListener>> m_dummies{};

	protected:
		friend class EventListener;

		NODISCARD auto next_id() noexcept { return ++m_next_id; }

	public:
		EventSystem() noexcept {}

		virtual ~EventSystem() noexcept override { remove_delegates(); }

		template <class Ev = Event
		> void fire_event(Ev && value) noexcept
		{
			if (auto const group{ m_listeners.find((EventID)value) })
			{
				for (auto const e : (*group->second))
				{
					VALIDATE(e)->on_event(FWD(value));
				}
			}
		}

	public:
		/* LISTENER API */

		template <class Ev> bool add_listener(EventListener * value) noexcept
		{
			return m_listeners[Ev::ID].insert(value).second;
		}

		template <class Ev> void remove_listener(EventListener * value) noexcept
		{
			if (auto const group{ m_listeners.find(Ev::ID) })
			{
				if (auto const e{ group->second->find(value) }; e != group->second->end())
				{
					group->second->erase(e);
				}
			}
		}

		void remove_listener(EventListener * value) noexcept
		{
			m_listeners.for_each([&](auto, FlatSet<EventListener *> & group) noexcept
			{
				if (auto const e{ group.find(value) }; e != group.end())
				{
					group.erase(e);
				}
			});
		}

	public:
		/* DUMMY API */

		NODISCARD auto const & get_all_dummy_listeners() const noexcept { return m_dummies; }

		template <class ... Evs, class ... Args
		> auto & add_dummy_listener(Args && ... args) noexcept
		{
			auto temp{ std::make_shared<DummyListener>(this, FWD(args)...) };

			if constexpr (0 < sizeof...(Evs)) { temp->subscribe<Evs...>(); }

			return m_dummies.emplace_back(std::move(temp));
		}

		auto remove_dummy_listener(std::shared_ptr<DummyListener> const & value) noexcept
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

		void remove_all_dummy_listeners() noexcept
		{
			while (!m_dummies.empty()) { m_dummies.pop_back(); }
		}

	public:
		/* DELEGATE API */

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

					static_assert(std::is_base_of_v<event, T>, "invalid event type");

					if (auto const it{ m_delegates.find(hash_v<T>) })
					{
						memdelete((EventDelegate<T> *)(*it->second));

						m_delegates.erase(it->first);
					}
				});
			}
			else
			{
				m_delegates.for_each([&](auto, auto * ptr) noexcept { memdelete(ptr); });

				m_delegates.clear();
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EVENT_SYSTEM_HPP_
