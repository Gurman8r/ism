#ifndef _ISM_OBJECT_API_HPP_
#define _ISM_OBJECT_API_HPP_

#include <core/object/detail/common.hpp>

// object_api
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : public _API_Tag
	{
		NODISCARD auto derived() const & noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		template <class Index = cstring
		> NODISCARD auto attr(Index && i) const { return AttrAccessor<Index>{ derived().ptr(), FWD(i) }; }

		template <class Index = cstring
		> NODISCARD auto operator[](Index && i) const { return ItemAccessor<Index>{ derived().ptr(), FWD(i) }; }

		template <class Value = OBJ
		> NODISCARD bool contains(Value && value) const { return attr("__contains__")(FWD(value)).cast<bool>(); }

		template <ReturnValuePolicy_ policy = ReturnValuePolicy_AutomaticReference, class ... Args
		> OBJ operator()(Args && ... args) const; // call.hpp

	public:
		NODISCARD bool is(ObjectAPI const & other) const noexcept { return derived().ptr() == other.derived().ptr(); }

		template <class O, class = std::enable_if_t<is_base_object_v<O>>
		> NODISCARD bool is(O const * other) const noexcept { return derived().ptr() == other; }

		template <class O, class = std::enable_if_t<is_api_v<O>>
		> NODISCARD bool is(O const & other) const noexcept { return derived().ptr() == other.ptr(); }
		
	public:
		NODISCARD bool equal_to(ObjectAPI const & other) const noexcept { return compare(other) == 0; }
		
		NODISCARD bool not_equal_to(ObjectAPI const & other) const noexcept { return compare(other) != 0; }
		
		NODISCARD bool less(ObjectAPI const & other) const noexcept { return compare(other) < 0; }
		
		NODISCARD bool less_equal(ObjectAPI const & other) const noexcept { return compare(other) <= 0; }
		
		NODISCARD bool greater(ObjectAPI const & other) const noexcept { return compare(other) > 0; }
		
		NODISCARD bool greater_equal(ObjectAPI const & other) const noexcept { return compare(other) >= 0; }

	private:
		NODISCARD auto compare(ObjectAPI const & o) const
		{
			Object * self{ derived().ptr() }, * other{ o.derived().ptr() };
			
			if (self == other)
			{
				return 0;
			}
			else if (self && other)
			{
				TYPE type{ typeof(self) };

				if (cmpfunc cf{ type->tp_cmp })
				{
					return cf(self, other);
				}
				else if (hashfunc hf{ type->tp_hash })
				{
					return util::compare(hf(self), hf(other));
				}
			}
			
			return CMP((intptr_t)self, (intptr_t)other);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD bool operator==(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.equal_to(b); }

	template <class T
	> NODISCARD bool operator!=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.not_equal_to(b); }

	template <class T
	> NODISCARD bool operator<(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less(b); }

	template <class T
	> NODISCARD bool operator<=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less_equal(b); }

	template <class T
	> NODISCARD bool operator>(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater(b); }

	template <class T
	> NODISCARD bool operator>=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater_equal(b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher<ObjectAPI<T>>
	{
		template <class U> hash_t operator()(U const & o) const { return hash(o); }
	};

	template <class T> struct EqualTo<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OBJECT_API_HPP_
