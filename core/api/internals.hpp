#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/api/object/base_object.hpp>

namespace ism
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

		Vector<OBJ> loader_life_support_stack; // loader life support stack

		OBJ modules; // module dictionary

	public:
		template <class T> auto & bind_class() { T::initialize_class(); return (*this); }

		void add_class(TYPE const & type);

		TYPE get_class(StringName const & name) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Internals & get_internals() noexcept { return *VALIDATE(Internals::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
