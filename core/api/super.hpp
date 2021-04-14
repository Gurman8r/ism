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
#include <core/templates/functional.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/set.hpp>
#include <core/templates/type_info.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_HANDLE(SuperID);

	class Super;
	class Reference;
	class Resource;

	template <class T> class Ref;
	ALIAS(REF) Ref<Reference>;
	ALIAS(RES) Ref<Resource>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct is_super : std::is_base_of<Super, T> {};
	template <class T> constexpr bool is_super_v{ ism::is_super<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Super
	{
	private:
		friend class SuperDB;
		friend class Reference;
		template <class T> friend class Ref;

		SuperID m_super_id{};
		int32_t m_predelete_ok{};
		bool m_is_reference{};

	protected:
		friend bool predelete_handler(Super *);
		friend void postinitialize_handler(Super *);
		virtual void _postinitialize();
		virtual bool _predelete();

	public:
		Super(bool is_ref = false);

		virtual ~Super();

		NODISCARD SuperID get_super_id() const { return m_super_id; }

		NODISCARD bool is_reference() const { return m_is_reference; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FORCE_INLINE void ism::postinitialize_handler(Super * value) { value->_postinitialize(); }

	FORCE_INLINE bool ism::predelete_handler(Super * value) { return value->_predelete(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From
	> NODISCARD auto super_cast(From from)
	{
		using U = intrinsic_t<From>;

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

	class ISM_API SuperDB final
	{
	private:
		friend class Super;

		static Batch<SuperID, Super *> g_supers;

		static SuperID add_instance(Super * value);

		static void remove_instance(SuperID value);

	public:
		static Super * get_instance(SuperID id);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SUPER_HPP_
