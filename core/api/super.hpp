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

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_HANDLE(SuperID);

	struct nullobj_t : std::integral_constant<SuperID, nullptr> { struct _Tag {}; constexpr explicit nullobj_t(_Tag) {} };

	inline constexpr nullobj_t nullobj{ nullobj_t::_Tag{} };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Super;
	class Reference;
	class Resource;
	class Script;

	template <class T> class Ref;
	ALIAS(REF) Ref<Reference>;
	ALIAS(RES) Ref<Resource>;
	ALIAS(SCR) Ref<Script>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct is_super : std::is_base_of<Super, T> {};
	template <class T> constexpr bool is_super_v{ ISM::is_super<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Super
	{
	private:
		friend class Reference;
		template <class T> friend class Ref;
		friend bool predelete_handler(Super *);
		friend void postinitialize_handler(Super *);

		void _postinitialize();
		bool _predelete();

		SuperID m_super_id{};
		int32_t m_predelete_ok{};
		bool m_is_reference{};

	public:
		Super(bool is_ref = false);

		virtual ~Super();

		NODISCARD SuperID get_super_id() const { return m_super_id; }

		NODISCARD bool is_reference() const { return m_is_reference; }

		static Super * get_super_instance(SuperID id);

	protected:
		static SuperID register_super_instance(Super * value);

		static void unregister_super_instance(Super * value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class = std::enable_if_t<is_super_v<T>>
	> NODISCARD T * super_cast(Super * from) { return dynamic_cast<T *>(from); }

	template <class T, class = std::enable_if_t<is_super_v<T>>
	> NODISCARD T const * super_cast(Super const * from) { return dynamic_cast<T const *>(from); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FORCE_INLINE void ISM::postinitialize_handler(Super * value) { value->_postinitialize(); }

	FORCE_INLINE bool ISM::predelete_handler(Super * value) { return value->_predelete(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SUPER_HPP_
