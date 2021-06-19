#ifndef _ISM_SUPER_HPP_
#define _ISM_SUPER_HPP_

#include <core/typedefs.hpp>
#include <core/string/path.hpp>
#include <core/math/color.hpp>
#include <core/math/quat.hpp>
#include <core/math/transform.hpp>
#include <core/math/transform_2d.hpp>
#include <core/math/vector4.hpp>
#include <core/math/rect.hpp>
#include <core/templates/any.hpp>
#include <core/templates/atomic.hpp>
#include <core/templates/ecs.hpp>
#include <core/templates/flat_map.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/set.hpp>
#include <core/templates/type_info.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_SUPER(m_class, m_inherits)																\
public:																								\
	NODISCARD virtual String get_class() const override												\
	{																								\
		return String{ TOSTR(m_class) };															\
	}																								\
																									\
	NODISCARD FORCE_INLINE static void * get_class_ptr_static()										\
	{																								\
		static int32_t ptr; return &ptr;															\
	}																								\
																									\
	NODISCARD FORCE_INLINE static constexpr StringView get_class_static()							\
	{																								\
		return TOSTR(m_class);																		\
	}																								\
																									\
	NODISCARD FORCE_INLINE static constexpr StringView get_parent_class_static()					\
	{																								\
		return m_inherits::get_class_static();														\
	}																								\
																									\
	NODISCARD static constexpr StringView inherits_static()											\
	{																								\
		return TOSTR(m_inherits);																	\
	}																								\
																									\
	NODISCARD virtual bool is_class(String const & value) const override							\
	{																								\
		return (value == TOSTR(m_class)) ? true : m_inherits::is_class(value);						\
	}																								\
																									\
	NODISCARD virtual bool is_class_ptr(void * value) const override								\
	{																								\
		return (value == m_class::get_class_ptr_static()) ? true : m_inherits::is_class_ptr(value);	\
	}																								\
																									\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_HANDLE(InstanceID);

	class ISM_API Super
	{
	private:
		friend class Reference;

		InstanceID m_instance_id;
		bool const m_is_reference;

		static InstanceID next_id() { static InstanceID id{}; return ++id; }

		Super(bool is_ref) noexcept : m_instance_id{ next_id() }, m_is_reference{ is_ref } {}

	protected:
		Super() noexcept : Super{ true } {}

	public:
		virtual ~Super() noexcept = default;

		NODISCARD bool is_reference() const noexcept { return m_is_reference; }

		NODISCARD InstanceID get_instance_id() const noexcept { return m_instance_id; }

		NODISCARD virtual String get_class() const { return "Super"; }

		NODISCARD static void * get_class_ptr_static() { static int32_t ptr; return &ptr; }

		NODISCARD static constexpr StringView get_class_static() { return "Super"; }

		NODISCARD static constexpr StringView get_parent_class_static() { return ""; }

		NODISCARD virtual bool is_class(String const & value) const { return value == "Super"; }

		NODISCARD virtual bool is_class_ptr(void * value) const { return value == get_class_ptr_static(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct is_super : std::is_base_of<Super, T> {};

	template <class T
	> constexpr bool is_super_v{ is_super<T>::value };

	template <class To, class From
	> NODISCARD auto super_cast(From from)
	{
		using T = mpl::intrinsic_t<To>;
		using F = mpl::intrinsic_t<From>;

		static_assert(!std::is_same_v<T, F>, "?");

		static_assert(is_super_v<T> && is_super_v<F>, "?");

		if constexpr (std::is_same_v<To, std::add_const_t<T>>)
		{
			return dynamic_cast<To const *>(const_cast<F const *>(from));
		}
		else
		{
			static_assert(std::is_same_v<To, T>, "?");

			return dynamic_cast<To *>(const_cast<F *>(from));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SUPER_HPP_
