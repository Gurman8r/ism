#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class Texture;

	class ISM_API Mesh : public Resource
	{
		OBJECT_COMMON(Mesh, Resource);

		enum { VAO, IBO, VBO, TEX };

		mutable Batch<RID, RID, RID, Vector<Ref<Texture>>> m_data{};

	public:
		Mesh() noexcept {}

		explicit Mesh(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Mesh() override;

		void draw() const;

		NODISCARD virtual RID get_rid() const override { return m_data.front<VAO>(); }

		virtual void reload_from_file() override;

	private:

	};
}

#endif // !_ISM_MESH_HPP_
