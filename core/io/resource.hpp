#ifndef _ISM_RESOURCE_HPP_
#define _ISM_RESOURCE_HPP_

#include <core/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Resource;

	ALIAS(RES) Ref<Resource>;

	OPAQUE_TYPE(RID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// resource
	class ISM_API Resource : public Object
	{
		OBJECT_COMMON(Resource, Object);

		RID		m_rid{};
		int32_t	m_index{};
		String	m_name{};
		String	m_path_cache{};

	protected:
		explicit Resource() noexcept;
		
	public:
		virtual ~Resource();

		virtual void reload_from_file();

		NODISCARD auto get_id() const -> RID { return m_rid; }

		NODISCARD auto get_index() const -> int32_t { return m_index; }
		
		NODISCARD auto get_name() const -> String const & { return m_name; }
		
		NODISCARD auto get_path() const -> String const & { return m_path_cache; }

		void set_index(int32_t value) { m_index = value; }

		void set_name(String const & value) { m_name = value; }

		virtual void set_path(String const & value, bool take_over = true);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
