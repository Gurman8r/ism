#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	class ISM_API Mesh : public Resource
	{
		OBJECT_COMMON(Mesh, Resource);

		friend class MeshLoader;

		RID m_mesh{};

	public:
		Mesh() noexcept {}

		explicit Mesh(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Mesh() override;

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_rid() const override { return m_mesh; }

		NODISCARD virtual size_t get_surface_count() const;

		NODISCARD virtual RS::Primitive_ surface_get_primitive(size_t index) const;

		NODISCARD virtual RID surface_get_vertex_array(size_t index) const;

		NODISCARD virtual RID surface_get_index_array(size_t index) const;

		NODISCARD virtual RID surface_get_material(size_t index) const;
	};
}

#endif // !_ISM_MESH_HPP_
