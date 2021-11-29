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

		Vector<OBJ> loader_stack; // loader life support stack

		OBJ modules; // module dictionary

	public:
		template <class First, class ... Rest> void bind_class()
		{
			mpl::for_types<First, Rest...>([&](auto tag)
			{
				using T = TAG_TYPE(tag);

				T::initialize_class();
			});
		}

		void add_class(TYPE const & type);

		TYPE get_class(StringName const & name) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
