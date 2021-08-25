#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/api/object/base_object.hpp>

namespace ism
{
	class ISM_API Internals final
	{
		static Internals singleton;

		Internals() noexcept = default;

		~Internals() noexcept = default;

	public:
		OBJ dict, builtins, modules, importlib;

		OBJ import_func;

		Batch<hash_t, StringName, TYPE> classes;

		Vector<OBJ> loader_stack;

	public:
		NODISCARD static Internals & get_singleton() noexcept { return singleton; }

		void initialize();

		void finalize();

	public:
		void add_class(TYPE type);

		template <class T> static void bind_class() { T::initialize_class(); }
	};

	NODISCARD inline Internals & get_internals() noexcept { return Internals::get_singleton(); }
}

#endif // !_ISM_INTERNALS_HPP_
