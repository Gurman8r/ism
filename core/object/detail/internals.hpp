#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/object/base_object.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Internals final
	{
		static Internals * __singleton;

		friend class Main;

		Internals();

	public:
		~Internals();

		FORCE_INLINE static Internals * get_singleton() noexcept { return __singleton; }

	public:
		/* MISC */

		Vector<OBJ> loader_stack; // loader life support stack

		OBJ modules; // module dictionary

	public:
		/* CLASS DATABASE */

		Batch<hash_t, StringName, TYPE> class_db;

		template <class First, class ... Rest
		> void initialize_class() noexcept
		{
			mpl::for_types<First, Rest...>([&](auto tag) noexcept
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
