#ifndef _ISM_RESOURCE_HPP_
#define _ISM_RESOURCE_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OPAQUE_TYPE(RID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// resource
	class ISM_API Resource : public Object
	{
		OBJECT_CLASS(Resource, Object);

		i32 m_subindex{};
		String m_name{};
		Path m_path_cache{};

	protected:
		Resource() noexcept {}
		
	public:
		virtual ~Resource() noexcept override = default;

		virtual Error_ reload_from_file();

		virtual RID get_rid() const { return {}; }

		i32 get_subindex() const { return m_subindex; }
		
		String const & get_name() const { return m_name; }

		Path const & get_path() const { return m_path_cache; }

		void set_subindex(i32 value) { m_subindex = value; }

		void set_name(String const & value) { m_name = value; }

		virtual void set_path(Path const & value, bool take_over = true);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(RES) Ref<Resource>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
