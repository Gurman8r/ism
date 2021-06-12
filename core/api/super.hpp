#ifndef _ISM_SUPER_HPP_
#define _ISM_SUPER_HPP_

#include <core/typedefs.hpp>
#include <core/string/path.hpp>
#include <core/math/color.hpp>
#include <core/math/quat.hpp>
#include <core/math/transform.hpp>
#include <core/math/transform_2d.hpp>
#include <core/math/vector4.hpp>
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

	DECL_HANDLE(InstanceID);

	class Super;
	class Reference;
	class Resource;

	template <class T> class Ref;
	ALIAS(REF) Ref<Reference>;
	ALIAS(RES) Ref<Resource>;

	template <class T> struct is_super : std::is_base_of<Super, T> {};

	template <class T> constexpr bool is_super_v{ ism::is_super<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_SUPER_CLASS(m_class, m_inherits)														\
protected:																							\
	using self_type = m_class;																		\
	using base_type = m_inherits;																	\
																									\
private:																							\
	mutable StringName m_class_name;																\
																									\
public:																								\
	NODISCARD virtual String get_class() const override												\
	{																								\
		return String{ TOSTR(m_class) };															\
	}																								\
																									\
	NODISCARD virtual StringName const * _get_class_namev() const override							\
	{																								\
		if (!m_class_name) { m_class_name = m_class::get_class_static(); }							\
		return &m_class_name;																		\
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
	static void get_inheritance_list_static(Vector<String> * value)									\
	{																								\
		m_inherits::get_inheritance_list_static(value);												\
		value->push_back(String{ TOSTR(m_class) });													\
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

	class ISM_API Super
	{
	private:
		friend bool predelete_handler(Super *);
		friend void postinitialize_handler(Super *);

		static InstanceID g_superID;

		bool _predelete();

		void _postinitialize();

		void _construct_super(bool is_ref);

		void _destruct_super();

	protected:
		InstanceID	m_instance_id{};
		uint8_t		m_predelete_ok{};
		bool		m_is_reference{};

		mutable StringName m_class_name{}, * m_class_ptr{};

	protected:
		FORCE_INLINE virtual StringName const * _get_class_namev() const
		{
			if (!m_class_name) { m_class_name = get_class_static(); }
			return &m_class_name;
		}

		Super(bool is_ref) { _construct_super(is_ref); }

		Super() { _construct_super(false); }

	public:
		virtual ~Super() noexcept { _destruct_super(); }

		NODISCARD bool is_reference() const { return m_is_reference; }

		NODISCARD InstanceID get_instance_id() const { return m_instance_id; }

		NODISCARD static void * get_class_ptr_static() { static int32_t ptr; return &ptr; }

		static void get_inheritance_list_static(Vector<String> * value) { value->push_back("Super"); }

		NODISCARD static constexpr StringView get_parent_class_static() { return ""; }

		NODISCARD static constexpr StringView get_class_static() { return "Super"; }

		NODISCARD virtual String get_class() const { return "Super"; }

		NODISCARD virtual bool is_class(String const & value) const { return value == "Super"; }

		NODISCARD virtual bool is_class_ptr(void * value) const { return value == get_class_ptr_static(); }

		NODISCARD StringName const & get_class_name() const { return !m_class_ptr ? *_get_class_namev() : *m_class_ptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FORCE_INLINE bool ism::predelete_handler(Super * value) { return value->_predelete(); }

	FORCE_INLINE void ism::postinitialize_handler(Super * value) { value->_postinitialize(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From
	> NODISCARD auto super_cast(From from)
	{
		using U = mpl::intrinsic_t<From>;

		static_assert(is_super_v<To> && is_super_v<U>);

		if constexpr (std::is_same_v<decltype(*from), U const &>)
		{
			return dynamic_cast<To const *>(from);
		}
		else
		{
			return dynamic_cast<To *>(from);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SUPER_HPP_
