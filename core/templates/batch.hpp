#ifndef _ISM_BATCH_HPP_
#define _ISM_BATCH_HPP_

#include <core/templates/vector.hpp>
#include <core/templates/mpl.hpp>

namespace ISM
{
	// tuple<vector<Ts>...>
	template <class ... _Ts
	> class Batch
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename Batch<_Ts...>;
		using allocator_type	= typename PolymorphicAllocator<byte>;
		using value_types		= typename MPL::type_list<_Ts...>;
		using value_tuple		= typename MPL::tuple<value_types>;
		using vector_types		= typename MPL::remap<ISM::Vector, value_types>;
		using vector_tuple		= typename MPL::tuple<vector_types>;
		using init_type			= typename std::initializer_list<value_tuple>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t npos
		{
			static_cast<size_t>(-1)
		};

		static constexpr size_t tuple_size
		{
			std::tuple_size_v<value_tuple>
		};

		static constexpr auto tuple_sequence
		{
			std::make_index_sequence<tuple_size>{}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I>	using value_i			= typename MPL::nth<I, value_types>;
		template <class  T> using value_t			= typename T;
		template <size_t I>	using vector_i			= typename MPL::nth<I, vector_types>;
		template <class  T> using vector_t			= typename ISM::Vector<T>;
		template <size_t I>	using iterator_i		= typename vector_i<I>::iterator;
		template <class  T>	using iterator_t		= typename vector_t<T>::iterator;
		template <size_t I>	using const_iterator_i	= typename vector_i<I>::const_iterator;
		template <class  T>	using const_iterator_t	= typename vector_t<T>::const_iterator;

		using iterator_types		= typename MPL::remap<iterator_t, value_types>;
		using iterator_tuple		= typename MPL::tuple<iterator_types>;

		using const_iterator_types	= typename MPL::remap<const_iterator_t, value_types>;
		using const_iterator_tuple	= typename MPL::tuple<const_iterator_types>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Batch() noexcept
			: m_data{}
		{
		}

		Batch(allocator_type alloc) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
		}

		Batch(init_type value, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
			this->reserve(value.size());

			for (auto const & elem : value)
			{
				this->emplace_back(elem);
			}
		}

		explicit Batch(vector_tuple const & value, allocator_type alloc = {})
			: m_data{ std::allocator_arg, alloc, value }
		{
		}

		explicit Batch(vector_tuple && value, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc, std::move(value) }
		{
		}

		Batch(self_type const & value, allocator_type alloc = {})
			: m_data{ std::allocator_arg, alloc, value.m_data }
		{
		}

		Batch(self_type && value, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc, std::move(value.m_data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(init_type value)
		{
			self_type temp{ init };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(vector_tuple const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(vector_tuple && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD decltype(auto) data() & noexcept { return m_data; }

		NODISCARD decltype(auto) data() const & noexcept { return m_data; }

		NODISCARD decltype(auto) data() && noexcept { return std::move(m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD decltype(auto) get() & noexcept
		{
			return std::get<I>(m_data);
		}

		template <size_t I> NODISCARD decltype(auto) get() const & noexcept
		{
			return std::get<I>(m_data);
		}

		template <size_t I> NODISCARD decltype(auto) get() && noexcept
		{
			return std::move(std::get<I>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> NODISCARD decltype(auto) get() & noexcept
		{
			return std::get<vector_t<T>>(m_data);
		}

		template <class T> NODISCARD decltype(auto) get() const & noexcept
		{
			return std::get<vector_t<T>>(m_data);
		}

		template <class T> NODISCARD decltype(auto) get() && noexcept
		{
			return std::move(std::get<vector_t<T>>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is
		> NODISCARD decltype(auto) get(size_t const i, std::index_sequence<Is...>) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) get(size_t const i, std::index_sequence<Is...>) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) get(size_t const i) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) get(size_t const i) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <class ... Ts
		> NODISCARD decltype(auto) get(size_t const i) noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>()[i]...);
			}
		}

		template <class ... Ts
		> NODISCARD decltype(auto) get(size_t const i) const noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>()[i]...);
			}
		}

		NODISCARD decltype(auto) get(size_t const i) noexcept
		{
			return this->get(i, tuple_sequence);
		}

		NODISCARD decltype(auto) get(size_t const i) const noexcept
		{
			return this->get(i, tuple_sequence);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is
		> NODISCARD decltype(auto) back(std::index_sequence<Is...>) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) back(std::index_sequence<Is...>) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) back() noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) back() const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <class ... Ts
		> NODISCARD decltype(auto) back() noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().back()...);
			}
		}

		template <class ... Ts
		> NODISCARD decltype(auto) back() const noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().back()...);
			}
		}

		NODISCARD decltype(auto) back() noexcept
		{
			return this->back(tuple_sequence);
		}

		NODISCARD decltype(auto) back() const noexcept
		{
			return this->back(tuple_sequence);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is
		> NODISCARD decltype(auto) front(std::index_sequence<Is...>) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) front(std::index_sequence<Is...>) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) front() noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <size_t ... Is
		> NODISCARD decltype(auto) front() const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <class ... Ts
		> NODISCARD decltype(auto) front() noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().front()...);
			}
		}

		template <class ... Ts
		> NODISCARD decltype(auto) front() const noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().front()...);
			}
		}

		NODISCARD decltype(auto) front() noexcept
		{
			return this->front(tuple_sequence);
		}

		NODISCARD decltype(auto) front() const noexcept
		{
			return this->front(tuple_sequence);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, size_t ... Is
		> void expand(Fn && fn, std::index_sequence<Is...>) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <class Fn, size_t ... Is
		> void expand(Fn && fn, std::index_sequence<Is...>) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <size_t ... Is, class Fn
		> void expand(Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <size_t ... Is, class Fn
		> void expand(Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <class ... Ts, class Fn
		> void expand(Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>()...);
		}

		template <class ... Ts, class Fn
		> void expand(Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>()...);
		}

		template <class Fn> void expand_all(Fn && fn) noexcept
		{
			this->expand(FWD(fn), tuple_sequence);
		}

		template <class Fn> void expand_all(Fn && fn) const noexcept
		{
			this->expand(FWD(fn), tuple_sequence);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, size_t ... Is
		> void expand(size_t const i, Fn && fn, std::index_sequence<Is...>) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <class Fn, size_t ... Is
		> void expand(size_t const i, Fn && fn, std::index_sequence<Is...>) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <size_t ... Is, class Fn
		> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <size_t ... Is, class Fn
		> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <class ... Ts, class Fn
		> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>(i)...);
		}

		template <class ... Ts, class Fn
		> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>(i)...);
		}

		template <class Fn> void expand_all(size_t const i, Fn && fn) noexcept
		{
			this->expand(i, FWD(fn), tuple_sequence);
		}

		template <class Fn> void expand_all(size_t const i, Fn && fn) const noexcept
		{
			this->expand(i, FWD(fn), tuple_sequence);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_tuple(Fn && fn) noexcept
		{
			MPL::for_tuple(m_data, FWD(fn));
		}

		template <class Fn
		> void for_tuple(Fn && fn) const noexcept
		{
			MPL::for_tuple(m_data, FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn
		> void for_indices(Fn && fn) noexcept
		{
			this->expand<Is...>([&](auto && ... vs) noexcept
			{
				MPL::for_args([&](auto & v) noexcept
				{
					std::invoke(FWD(fn), v);
				}
				, FWD(vs)...);
			});
		}

		template <class ... Ts, class Fn
		> void for_types(Fn && fn) noexcept
		{
			this->expand<Ts...>([&](auto && ... vs) noexcept
			{
				MPL::for_args([&](auto & v) noexcept
				{
					std::invoke(FWD(fn), v);
				}
				, FWD(vs)...);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> NODISCARD size_t capacity() const noexcept
		{
			return this->get<I>().capacity();
		}

		template <class T> NODISCARD size_t capacity() const noexcept
		{
			return this->get<T>().capacity();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> NODISCARD bool empty() const noexcept
		{
			return this->get<I>().empty();
		}

		template <class T> NODISCARD bool empty() const noexcept
		{
			return this->get<T>().empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> NODISCARD size_t max_size() const noexcept
		{
			return this->get<I>().max_size();
		}

		template <class T> NODISCARD size_t max_size() const noexcept
		{
			return this->get<T>().max_size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> NODISCARD size_t size() const noexcept
		{
			return this->get<I>().size();
		}

		template <class T> NODISCARD size_t size() const noexcept
		{
			return this->get<T>().size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.clear(); });
		}

		template <size_t ... Is> void clear() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.clear(); });
		}

		template <class ... Ts> void clear() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.clear(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void pop_back() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.pop_back(); });
		}

		template <size_t ... Is> void pop_back() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.pop_back(); });
		}

		template <class ... Ts> void pop_back() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.pop_back(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reserve(size_t const value)
		{
			this->for_tuple([&](auto & v) { v.reserve(value); });
		}

		template <size_t ... Is> void reserve(size_t const value)
		{
			this->for_indices<Is...>([&](auto & v) { v.reserve(value); });
		}

		template <class ... Ts> void reserve(size_t const value)
		{
			this->for_types<Ts...>([&](auto & v) { v.reserve(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void resize(size_t const value)
		{
			this->for_tuple([&](auto & v) { v.resize(value); });
		}

		template <size_t ... Is> void resize(size_t const value)
		{
			this->for_indices<Is...>([&](auto & v) { v.resize(value); });
		}

		template <class ... Ts> void resize(size_t const value)
		{
			this->for_types<Ts...>([&](auto & v) { v.resize(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void shrink_to_fit() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.shrink_to_fit(); });
		}

		template <size_t ... Is> void shrink_to_fit() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.shrink_to_fit(); });
		}

		template <class ... Ts> void shrink_to_fit() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.shrink_to_fit(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void erase(size_t const i)
		{
			this->for_tuple([&](auto & v)
			{
				v.erase(v.begin() + i);
			});
		}

		void erase(size_t const first, size_t const last)
		{
			this->for_tuple([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <size_t ... Is> void erase(size_t const i)
		{
			this->for_indices<Is...>([&](auto & v)
			{
				v.erase(v.begin() + i);
			});
		}

		template <size_t ... Is> void erase(size_t const first, size_t const last)
		{
			this->for_indices<Is...>([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <class ... Ts> void erase(size_t const i)
		{
			this->for_types<Ts...>([&](auto & v)
			{
				v.erase(v.begin() + i);
			});
		}

		template <class ... Ts> void erase(size_t const first, size_t const last)
		{
			this->for_types<Ts...>([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = value_i<I>
		> NODISCARD iterator_i<I> find(U && value) noexcept
		{
			return std::find(this->begin<I>(), this->end<I>(), FWD(value));
		}

		template <size_t I, class U = value_i<I>
		> NODISCARD const_iterator_i<I> find(U && value) const noexcept
		{
			return std::find(this->cbegin<I>(), this->cend<I>(), FWD(value));
		}

		template <class T, class U = T
		> NODISCARD iterator_t<T> find(U && value) noexcept
		{
			return std::find(this->begin<T>(), this->end<T>(), FWD(value));
		}

		template <class T, class U = T
		> NODISCARD const_iterator_t<T> find(U && value) const noexcept
		{
			return std::find(this->cbegin<T>(), this->cend<T>(), FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class Pr
		> NODISCARD iterator_i<I> find_if(Pr && pr) noexcept
		{
			return std::find_if(this->begin<I>(), this->end<I>(), FWD(pr));
		}

		template <size_t I, class Pr
		> NODISCARD const_iterator_i<I> find_if(Pr && pr) const noexcept
		{
			return std::find_if(this->cbegin<I>(), this->cend<I>(), FWD(pr));
		}

		template <class T, class Pr
		> NODISCARD iterator_t<T> find_if(Pr && pr) noexcept
		{
			return std::find_if(this->begin<T>(), this->end<T>(), FWD(pr));
		}

		template <class T, class Pr
		> NODISCARD const_iterator_t<T> find_if(Pr && pr) const noexcept
		{
			return std::find_if(this->cbegin<T>(), this->cend<T>(), FWD(pr));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = value_i<I>
		> NODISCARD bool binary_search(U && value) const noexcept
		{
			return std::binary_search(this->cbegin<I>(), this->cend<I>(), FWD(value));
		}

		template <class T, class U = T
		> NODISCARD bool binary_search(U && value) const noexcept
		{
			return std::binary_search(this->cbegin<T>(), this->cend<T>(), FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = value_i<I>
		> NODISCARD bool contains(U && value) const noexcept
		{
			return this->end<T>() != this->find<I>(FWD(value));
		}

		template <class T, class U = T
		> NODISCARD bool contains(U && value) const noexcept
		{
			return this->end<T>() != this->find<T>(FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD size_t index_of(const_iterator_i<I> it) const noexcept
		{
			return (size_t)std::distance(this->cbegin<I>(), it);
		}

		template <class T> NODISCARD size_t index_of(const_iterator_t<T> it) const noexcept
		{
			return (size_t)std::distance(this->cbegin<T>(), it);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = value_i<I>
		> NODISCARD size_t lookup(U && value) const noexcept
		{
			if (auto const it{ this->find<I>(FWD(value)) }; it == this->end<I>()) { return npos; }
			else { return this->index_of<I>(it); }
		}

		template <class T, class U = value_t<T>
		> NODISCARD size_t lookup(U && value) const noexcept
		{
			if (auto const it{ this->find<T>(FWD(value)) }; it == this->end<T>()) { return npos; }
			else { return this->index_of<T>(it); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class Pr
		> NODISCARD size_t lookup_if(Pr && pr) const noexcept
		{
			if (auto const it{ this->find_if<I>(FWD(pr)) }; it == this->end<I>()) { return npos; }
			else { return this->index_of<I>(it); }
		}

		template <class T, class Pr
		> NODISCARD size_t lookup_if(Pr && pr) const noexcept
		{
			if (auto const it{ this->find_if<T>(FWD(pr)) }; it == this->end<T>()) { return npos; }
			else { return this->index_of<T>(it); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It = const_iterator_i<I>, class ... Args
		> auto emplace(It && it, Args && ... args) noexcept -> iterator_i<I>
		{
			return this->get<I>().emplace
			(
				this->get_iterator<I>(FWD(it)), FWD(args)...
			);
		}

		template <class T, class It = const_iterator_t<T>, class ... Args
		> auto emplace(It && it, Args && ... args) noexcept -> iterator_t<T>
		{
			return this->get<I>().emplace
			(
				this->get_iterator<T>(FWD(it)), FWD(args)...
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It = const_iterator_i<I>, class U = value_i<I>
		> auto insert(It && it, U && value) noexcept -> iterator_i<I>
		{
			return this->get<I>().emplace
			(
				this->get_iterator<I>(FWD(it)), FWD(value)
			);
		}

		template <class T, class It = const_iterator_t<T>, class U = value_t<T>
		> auto insert(It && it, U && value) noexcept -> iterator_t<T>
		{
			return this->get<T>().emplace
			(
				this->get_iterator<T>(FWD(it)), FWD(value)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = value_i<I>
		> decltype(auto) push_back(U && value) noexcept
		{
			return this->get<I>().emplace_back(FWD(value));
		}

		template <class T, class U = value_t<T>
		> decltype(auto) push_back(U && value) noexcept
		{
			return this->get<T>().emplace_back(FWD(value));
		}

		template <class Arg0, class ... Args
		> decltype(auto) push_back(Arg0 && arg0, Args && ... args) noexcept
		{
			return this->impl_push_back<0, value_tuple, tuple_size>
			(
				value_tuple{ FWD(arg0), FWD(args)... }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_data.swap(value.m_data);
			}
		}

		void swap(vector_tuple & value) noexcept
		{
			if (std::addressof(m_data) != std::addressof(value))
			{
				m_data.swap(value);
			}
		}

		template <size_t I, class U = vector_i<I>
		> void swap(U & value) noexcept
		{
			this->get<I>().swap(value);
		}

		template <class T, class U = T
		> void swap(U & value) noexcept
		{
			this->get<T>().swap(value);
		}

		void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_tuple([&](auto & v) noexcept { std::swap(v[lhs], v[rhs]); });
		}

		template <size_t ... Is
		> void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { std::swap(v[lhs], v[rhs]); });
		}

		template <class ... Ts
		> void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { std::swap(v[lhs], v[rhs]); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t From, size_t To
		> NODISCARD auto map_unchecked(value_i<From> const & from) -> value_i<To>
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <size_t From, size_t To
		> NODISCARD auto map_unchecked(value_i<From> const & from) const -> value_i<To> const &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <size_t From, class To
		> NODISCARD auto map_unchecked(value_i<From> const & from) -> To &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <size_t From, class To
		> NODISCARD auto map_unchecked(value_i<From> const & from) const -> To const &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <class From, size_t To
		> NODISCARD auto map_unchecked(From const & from) -> value_i<To> &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <class From, size_t To
		> NODISCARD auto map_unchecked(From const & from) const -> value_i<To> const &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <class From, class To
		> NODISCARD auto map_unchecked(From const & from) -> To &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		template <class From, class To
		> NODISCARD auto map_unchecked(From const & from) const -> To const &
		{
			return this->get<To>(this->lookup<From>(from));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t From, size_t To
		> NODISCARD auto map(value_i<From> const & from) -> value_i<To> *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <size_t From, size_t To
		> NODISCARD auto map(value_i<From> const & from) const -> value_i<To> const *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <size_t From, class To
		> NODISCARD auto map(value_i<From> const & from) -> To *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <size_t From, class To
		> NODISCARD auto map(value_i<From> const & from) const -> To const *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, size_t To
		> NODISCARD auto map(From const & from) -> value_i<To> *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, size_t To
		> NODISCARD auto map(From const & from) const -> value_i<To> const *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, class To
		> NODISCARD auto map(From const & from) -> To *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, class To
		> NODISCARD auto map(From const & from) const -> To const *
		{
			if (size_t const i{ this->lookup<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It = iterator_i<I>
		> NODISCARD auto get_iterator(It && it) const noexcept -> iterator_i<I>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->begin<I>() + it;
			}
			else
			{
				return it;
			}
		}

		template <class T, class It = iterator_t<T>
		> NODISCARD auto get_iterator(It && it) const noexcept -> iterator_t<T>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->begin<T>() + it;
			}
			else
			{
				return it;
			}
		}

		template <size_t I, class It = const_iterator_i<I>
		> NODISCARD auto get_const_iterator(It && it) const noexcept -> const_iterator_i<I>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->cbegin<I>() + it;
			}
			else
			{
				return it;
			}
		}

		template <class T, class It = const_iterator_t<T>
		> NODISCARD auto get_const_iterator(It && it) const noexcept -> const_iterator_t<T>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->cbegin<T>() + it;
			}
			else
			{
				return it;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD auto begin() noexcept { return this->get<I>().begin(); }

		template <size_t I> NODISCARD auto begin() const noexcept { return this->get<I>().begin(); }

		template <size_t I> NODISCARD auto cbegin() const noexcept { return this->get<I>().cbegin(); }

		template <class T> NODISCARD auto begin() noexcept { return this->get<T>().begin(); }

		template <class T> NODISCARD auto begin() const noexcept { return this->get<T>().begin(); }

		template <class T> NODISCARD auto cbegin() const noexcept { return this->get<T>().cbegin(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD auto end() noexcept { return this->get<I>().end(); }

		template <size_t I> NODISCARD auto end() const noexcept { return this->get<I>().end(); }

		template <size_t I> NODISCARD auto cend() const noexcept { return this->get<I>().cend(); }

		template <class T> NODISCARD auto end() noexcept { return this->get<T>().end(); }

		template <class T> NODISCARD auto end() const noexcept { return this->get<T>().end(); }

		template <class T> NODISCARD auto cend() const noexcept { return this->get<T>().cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD auto rbegin() noexcept { return this->get<I>().rbegin(); }

		template <size_t I> NODISCARD auto rbegin() const noexcept { return this->get<I>().rbegin(); }

		template <size_t I> NODISCARD auto crbegin() const noexcept { return this->get<I>().crbegin(); }

		template <class T> NODISCARD auto rbegin() noexcept { return this->get<T>().rbegin(); }

		template <class T> NODISCARD auto rbegin() const noexcept { return this->get<T>().rbegin(); }

		template <class T> NODISCARD auto crbegin() const noexcept { return this->get<T>().crbegin(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD auto rend() noexcept { return this->get<I>().rend(); }

		template <size_t I> NODISCARD auto rend() const noexcept { return this->get<I>().rend(); }

		template <size_t I> NODISCARD auto crend() const noexcept { return this->get<I>().crend(); }

		template <class T> NODISCARD auto rend() noexcept { return this->get<T>().rend(); }

		template <class T> NODISCARD auto rend() const noexcept { return this->get<T>().rend(); }

		template <class T> NODISCARD auto crend() const noexcept { return this->get<T>().crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD auto begin_move_iterator() noexcept { return std::make_move_iterator(this->begin<I>()); }

		template <size_t I> NODISCARD auto begin_move_iterator() const noexcept { return std::make_move_iterator(this->begin<I>()); }

		template <size_t I> NODISCARD auto cbegin_move_iterator() const noexcept { return std::make_move_iterator(this->cbegin<I>()); }

		template <size_t I> NODISCARD auto end_move_iterator() noexcept { return std::make_move_iterator(this->end<I>()); }

		template <size_t I> NODISCARD auto end_move_iterator() const noexcept { return std::make_move_iterator(this->end<I>()); }

		template <size_t I> NODISCARD auto cend_move_iterator() const noexcept { return std::make_move_iterator(this->cend<I>()); }

		template <class T> NODISCARD auto begin_move_iterator() noexcept { return std::make_move_iterator(this->begin<T>()); }

		template <class T> NODISCARD auto begin_move_iterator() const noexcept { return std::make_move_iterator(this->begin<T>()); }

		template <class T> NODISCARD auto cbegin_move_iterator() const noexcept { return std::make_move_iterator(this->cbegin<T>()); }

		template <class T> NODISCARD auto end_move_iterator() noexcept { return std::make_move_iterator(this->end<T>()); }

		template <class T> NODISCARD auto end_move_iterator() const noexcept { return std::make_move_iterator(this->end<T>()); }

		template <class T> NODISCARD auto cend_move_iterator() const noexcept { return std::make_move_iterator(this->cend<T>()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = self_type
		> NODISCARD auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, self_type>)
			{
				return this->compare(value.m_data);
			}
			else
			{
				static_assert(std::is_same_v<U, vector_tuple>);
				return ML_compare(m_data, value);
			}
		}

		template <class U = self_type
		> NODISCARD bool operator==(U const & value) const noexcept
		{
			return this->compare(value) == 0;
		}

		template <class U = self_type
		> NODISCARD bool operator!=(U const & value) const noexcept
		{
			return this->compare(value) != 0;
		}

		template <class U = self_type
		> NODISCARD bool operator<(U const & value) const noexcept
		{
			return this->compare(value) < 0;
		}

		template <class U = self_type
		> NODISCARD bool operator>(U const & value) const noexcept
		{
			return this->compare(value) > 0;
		}

		template <class U = self_type
		> NODISCARD bool operator<=(U const & value) const noexcept
		{
			return this->compare(value) <= 0;
		}

		template <class U = self_type
		> NODISCARD bool operator>=(U const & value) const noexcept
		{
			return this->compare(value) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
	protected:
		// push_back implementation
		template <size_t Idx, class Tpl, size_t Max
		> decltype(auto) impl_push_back(Tpl && value) noexcept
		{
			static_assert(self_type::tuple_size <= Max);

			if constexpr (Idx < Max)
			{
				this->push_back<Idx>(std::get<Idx>(FWD(value)));

				return this->impl_push_back<Idx + 1, Tpl, Max>(FWD(value));
			}
			else
			{
				return this->back();
			}
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vector_tuple m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_BATCH_HPP_
