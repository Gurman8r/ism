#ifndef _ISM_RESOURCE_HPP_
#define _ISM_RESOURCE_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Resource;

	ALIAS(RES) Ref<Resource>;

	DECL_HANDLE(RID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// resource
	class ISM_API Resource : public Object
	{
		OBJECT_COMMON(Resource, Object);

		int32_t	m_index{};
		String	m_name{};
		String	m_path_cache{};

	protected:
		explicit Resource() noexcept;
		
	public:
		virtual ~Resource();

		virtual void reload_from_file();

		NODISCARD virtual RID get_rid() const { return {}; }

		NODISCARD int32_t get_index() const { return m_index; }
		
		NODISCARD String const & get_name() const { return m_name; }

		NODISCARD String const & get_path() const { return m_path_cache; }

		void set_index(int32_t value) { m_index = value; }

		void set_name(String const & value) { m_name = value; }

		virtual void set_path(String const & value, bool take_over = true);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
