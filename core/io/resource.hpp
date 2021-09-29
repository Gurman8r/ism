#ifndef _ISM_RESOURCE_HPP_
#define _ISM_RESOURCE_HPP_

#include <core/object/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Resource;

	ALIAS(RES) Ref<Resource>;

	MAKE_OPAQUE(RID);

	class ISM_API Resource : public Object
	{
		OBJECT_COMMON(Resource, Object);

		RID m_resource_id{};

		String m_name{}, m_path_cache{};

		int32_t m_subindex{};

	protected:
		explicit Resource() noexcept;
		
	public:
		virtual ~Resource();

		virtual void reload_from_file();

	public:
		NODISCARD auto get_id() const -> RID { return m_resource_id; }
		
		NODISCARD auto get_name() const -> String const & { return m_name; }
		
		NODISCARD auto get_path() const -> String const & { return m_path_cache; }
		
		NODISCARD auto get_subindex() const -> int32_t { return m_subindex; }

		void set_name(String const & value) { m_name = value; }

		virtual void set_path(String const & value, bool take_over = false);

		void set_subindex(int32_t value) { m_subindex = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
