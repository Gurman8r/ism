#ifndef _ISM_MESH_LOADER_HPP_
#define _ISM_MESH_LOADER_HPP_

#include <scene/resources/mesh.hpp>

namespace ism
{
	class ISM_API MeshLoader final
	{
	public:
		/* load from file */
		static Error_ load_mesh(Mesh & mesh, Path const & path);
		static Error_ load_mesh(Ref<Mesh> mesh, Path const & path);
	};
}

#endif // !_ISM_MESH_LOADER_HPP_
