#ifndef _ISM_MESH_LIBRARY_HPP_
#define _ISM_MESH_LIBRARY_HPP_

#include <scene/resources/mesh.hpp>
#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class MeshFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(MeshFormatLoader, ResourceFormatLoader);
	public:
		static Error_ load_mesh(Ref<Mesh> mesh, String const & path);
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * r_out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MESH_LIBRARY_HPP_
