#ifndef _ISM_RESOURCE_HPP_
#define _ISM_RESOURCE_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Resource;

	ALIAS(RES) Handle<Resource>;

	DECL_POINTER(RID);

	class ISM_API Resource : public Object
	{
		OBJ_CLASS(Resource, Object);

	private:
		String m_name{}, m_path_cache{};

		int32_t m_subindex{};

	protected:
		Resource() noexcept : Object{} {}
		
	public:
		virtual ~Resource();

	public:
		virtual void reload_from_file();

		NODISCARD auto get_name() const -> String const & { return m_name; }
		void set_name(String const & value) { m_name = value; }

		NODISCARD auto get_path() const -> String const & { return m_path_cache; }
		virtual void set_path(String const & value, bool take_over = false);

		NODISCARD auto get_subindex() const -> int32_t { return m_subindex; }
		void set_subindex(int32_t value) { m_subindex = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
