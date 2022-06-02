#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class Texture;

	class ISM_API Mesh : public Resource
	{
		OBJECT_COMMON(Mesh, Resource);

		friend class MeshLoader;

		RID m_mesh{};

		Batch<
			RID, // vertex array
			RID, // index array
			Vector<Ref<Texture>> // textures
		> m_surface_data{};

	public:
		using SurfaceData = typename decltype(m_surface_data);

		enum { VA, IA, TEX };

		Mesh();

		explicit Mesh(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Mesh() override;

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_rid() const override { return m_surface_data.front<VA>(); }

	public:
		NODISCARD size_t get_surface_count() const noexcept { return m_surface_data.size(); }

		template <size_t ... I, class Fn
		> void get_surface(size_t const i, Fn && fn) noexcept { m_surface_data.expand<I...>(i, FWD(fn)); }

		template <class Fn
		> void for_each_surface(Fn && fn) { for (size_t i = 0; i < m_surface_data.size(); ++i) { m_surface_data.expand_all(i, FWD(fn)); } }
	};
}

#endif // !_ISM_MESH_HPP_
