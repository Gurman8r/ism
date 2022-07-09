#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/object/base_object.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Internals final
	{
		static Internals * __singleton;

	public:
		Internals();

		~Internals();

		NON_COPYABLE(Internals);

		NON_MOVABLE(Internals);

		FORCE_INLINE static Internals * get_singleton() noexcept { return __singleton; }

	public:
		Batch<hash_t, StringName, TYPE> class_db; // class database

		List<OBJ> loader_stack; // loader life support stack

		OBJ modules; // module dictionary

	public:
		template <class Class0, class ... Classes
		> void install_class()
		{
			mpl::for_types<Class0, Classes...>([&](auto tag) noexcept
			{
				TAG_TYPE(tag)::initialize_class();
			});
		}

		void add_class(TYPE const & type);

		TYPE get_class(StringName const & name) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
