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

		int32_t m_subindex{};
		String m_name{};
		Path m_path_cache{};

	protected:
		Resource() noexcept {}
		
	public:
		virtual ~Resource() noexcept override = default;

		virtual Error_ reload_from_file();

		NODISCARD virtual RID get_rid() const { return {}; }

		NODISCARD int32_t get_subindex() const { return m_subindex; }
		
		NODISCARD String const & get_name() const { return m_name; }

		NODISCARD Path const & get_path() const { return m_path_cache; }

		void set_subindex(int32_t value) { m_subindex = value; }

		void set_name(String const & value) { m_name = value; }

		virtual void set_path(Path const & value, bool take_over = true);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
