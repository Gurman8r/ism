#ifndef _ISM_CLASS_DB_HPP_
#define _ISM_CLASS_DB_HPP_

#include <core/api/object/base_object.hpp>

namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// class_db
	class ISM_API ClassDB final
	{
	public:
		static Batch<
			hash_t,
			StringName,
			TYPE
		> classes;

	public:
		static void add_class(StringName const & name, TypeObject * type);

		template <class T
		> static void add_class() { add_class(T::get_class_static(), T::get_type_static()); }

		template <class T
		> static void register_class()
		{
			T::initialize_class();

			TYPE t{ *classes.map<hash_t, TYPE>(hash(T::get_class_static())) };

			if (t.is_null())
			{
				FATAL("failed to register class");
			}
		}

		NODISCARD static bool class_exists(StringName const & name);

		NODISCARD static TYPE class_type(StringName const & name);

		template <class T> NODISCARD static TYPE class_type() { return class_type(T::get_class_static()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CLASS_DB_HPP_
