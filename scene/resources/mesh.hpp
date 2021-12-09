#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Mesh : public Resource
	{
		OBJECT_COMMON(Mesh, Resource);

		mutable RID			m_array{};
		mutable RID			m_indices{};
		mutable Vector<RID>	m_vertices{};

	public:
		Mesh() noexcept {}

		explicit Mesh(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Mesh() override;

		NODISCARD virtual RID get_rid() const override { return m_array; }

		virtual void reload_from_file() override;

	private:

	};
}

#endif // !_ISM_MESH_HPP_
