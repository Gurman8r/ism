#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Mesh : public Resource
	{
		OBJECT_COMMON(Mesh, Resource);

	public:
		virtual ~Mesh();

		Mesh();

	private:

	};
}

#endif // !_ISM_MESH_HPP_
