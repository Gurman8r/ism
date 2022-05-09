#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class Texture;

	class ISM_API Mesh : public Resource
	{
		OBJECT_COMMON(Mesh, Resource);

		RID m_mesh{};

		mutable Batch<RID, RID, Vector<Ref<Texture>>> m_data{};

	public:
		enum { Vertices, Indices, Textures };

		Mesh() noexcept {}

		explicit Mesh(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Mesh() override;

		NODISCARD virtual RID get_rid() const override { return m_data.front<Vertices>(); }

		virtual void reload_from_file() override;

	public:
		NODISCARD size_t get_size() const noexcept { return m_data.size(); }

		template <size_t ... I, class Fn
		> void expand(size_t const i, Fn && fn) noexcept { m_data.expand<I...>(i, FWD(fn)); }

		template <class Fn
		> void each(Fn && fn) { for (size_t i = 0; i < m_data.size(); ++i) { m_data.expand_all(i, FWD(fn)); } }
	};
}

#endif // !_ISM_MESH_HPP_
