#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/object/base_object.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Internals final
	{
		static Internals * singleton;

	public:
		Internals() noexcept { singleton = this; initialize(); }

		~Internals() noexcept { finalize(); }

		NON_COPYABLE(Internals);

		NON_MOVABLE(Internals);

		NODISCARD static Internals * get_singleton() noexcept { return singleton; }

		void initialize();

		void finalize();

	public:
		OBJ dict, builtins, modules, importlib;

		OBJ import_func;

		Vector<OBJ> loader_stack;

	public:
		Batch<hash_t, StringName, TYPE> class_db;

		void add_class(TYPE type);

		template <class T> auto bind_class() -> Internals & { T::initialize_class(); return (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Internals & get_internals() noexcept { return *Internals::get_singleton(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
