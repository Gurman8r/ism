#ifndef _ISM_BITSET_HPP_
#define _ISM_BITSET_HPP_

#include <core/templates/array.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// read flag at index
#define flag_read(value, index)				(((index) & (value)) == (index))

// set flag at index
#define flag_set(value, index)				((value) |= (index))

// clear flag at index
#define flag_clear(value, index)			((value) &= ~(index))

// conditional set or clear flag
#define flag_write(value, index, boolean)	((boolean) ? flag_set(value, index) : flag_clear(value, index))

// map between flag bits
#define flag_map(dst, dindex, src, sindex)	flag_write(dst, dindex, flag_read(src, sindex))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// read bit at index
#define bit_read(value, index)				flag_read(value >> index, 1)

// set bit at index
#define bit_set(value, index)				flag_set(value, 1 << index)

// clear bit at index
#define bit_clear(value, index)				flag_clear(value, 1 << index)

// conditional set or clear bit
#define bit_write(value, index, boolean)	flag_write(value, 1 << index, boolean)

// map between bits
#define bit_map(dst, dindex, src, sindex)	bit_write(dst, dindex, bit_read(src, sindex))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	template <size_t _Count = 64
	> struct Bitset
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t bit_count{ _Count };

		static_assert(0 < bit_count, "bit count negative or zero");

		using self_type = typename Bitset<bit_count>;

		using array_type = typename Array<bool, bit_count>;

		using value_type = typename std::conditional_t<
			bit_count <= sizeof(uint32_t) * 8,
			uint32_t,
			uint64_t
		>;

		static constexpr ptrdiff_t bits_per_word{ sizeof(value_type) * 8 };
		
		static constexpr ptrdiff_t word_count{ (bit_count - 1) / bits_per_word };

		using storage_type = typename Array<value_type, word_count + 1>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Bitset() noexcept : m_words{} {}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> constexpr Bitset(T const value) noexcept
			: m_words{ static_cast<value_type>(value) }
		{
		}

		constexpr Bitset(self_type const & value)
			: m_words{ value.m_words }
		{
		}

		constexpr Bitset(self_type && value) noexcept
			: m_words{ std::move(value.m_words) }
		{
		}

		template <size_t N
		> constexpr Bitset(Bitset<N> const & value)
			: m_words{ value.m_words }
		{
		}

		template <size_t N
		> constexpr Bitset(Bitset<N> && value) noexcept
			: m_words{ std::move(value.m_words) }
		{
		}

		constexpr Bitset(array_type const & value) noexcept
			: m_words{}
		{
			for (auto it = value.begin(); it != value.end(); ++it)
			{
				write(std::distance(value.begin(), it), *it);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> constexpr self_type & operator=(T const value) noexcept
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				ism::util::swap(m_words, value.m_words);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator bool() const noexcept { return m_words; }

		NODISCARD constexpr operator storage_type const & () const & noexcept { return m_words; }

		NODISCARD constexpr bool operator[](size_t const i) const noexcept { return read(i); }

		NODISCARD constexpr auto words() noexcept -> storage_type & { return m_words; }

		NODISCARD constexpr auto words() const noexcept -> storage_type const & { return m_words; }

		NODISCARD constexpr auto size() const noexcept -> size_t { return bit_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr bool read(size_t const i) const noexcept
		{
			return bit_read(m_words[i / bits_per_word], i % bits_per_word);
		}

		constexpr bool clear(size_t const i) noexcept
		{
			bool const temp{ this->read(i) };
			
			bit_clear(m_words[i / bits_per_word], i % bits_per_word);
			
			return temp;
		}

		constexpr bool set(size_t const i) noexcept
		{
			bool const temp{ !this->read(i) };
			
			bit_set(m_words[i / bits_per_word], i % bits_per_word);
			
			return temp;
		}

		constexpr bool write(size_t const i, bool const b) noexcept
		{
			return b ? this->set(i) : this->clear(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> NODISCARD constexpr bool read() const noexcept
		{
			return this->read(I);
		}

		template <size_t I> constexpr bool clear() noexcept
		{
			return this->clear(I);
		}

		template <size_t I> constexpr bool set() noexcept
		{
			return this->set(I);
		}

		template <size_t I> constexpr bool write(bool const b) noexcept
		{
			return this->write(I, b);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr array_type arr() const noexcept
		{
			array_type temp{};
			for (size_t i = 0; i < bit_count; ++i)
			{
				temp[i] = read(i);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = self_type
		> constexpr auto compare(Other const & other) const noexcept
		{
			if constexpr (std::is_same_v<Other, self_type>)
			{
				return CMP(m_words, other.m_words);
			}
			else
			{
				return this->compare(self_type{ other });
			}
		}

		template <class Other = self_type
		> NODISCARD constexpr bool operator==(Other const & other) const
		{
			return this->compare(other) == 0;
		}

		template <class Other = self_type
		> NODISCARD constexpr bool operator!=(Other const & other) const
		{
			return this->compare(other) != 0;
		}

		template <class Other = self_type
		> NODISCARD constexpr bool operator<(Other const & other) const
		{
			return this->compare(other) < 0;
		}

		template <class Other = self_type
		> NODISCARD constexpr bool operator>(Other const & other) const
		{
			return this->compare(other) > 0;
		}

		template <class Other = self_type
		> NODISCARD constexpr bool operator<=(Other const & other) const
		{
			return this->compare(other) <= 0;
		}

		template <class Other = self_type
		> NODISCARD constexpr bool operator>=(Other const & other) const
		{
			return this->compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_words;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_BITSET_HPP_
