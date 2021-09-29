#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/object/base_object.hpp>

namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Internals final
	{
		static Internals * singleton;

	public:
		Internals();

		~Internals();

		NON_COPYABLE(Internals);

		NON_MOVABLE(Internals);

		NODISCARD static Internals * get_singleton() noexcept { return singleton; }

	public:
		Batch<hash_t, StringName, TYPE> class_db; // class database

		Vector<OBJ> loader_stack; // life support stack

		OBJ modules; // module dictionary

	public:
		void add_class(TYPE type);

		template <class T> auto & bind_class() { T::initialize_class(); return (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Internals & get_internals() noexcept { return *VALIDATE(Internals::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
