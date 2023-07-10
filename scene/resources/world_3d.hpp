#ifndef _ISM_WORLD_3D_HPP_
#define _ISM_WORLD_3D_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API World3D : public Resource
	{
		OBJECT_CLASS(World3D, Resource);

	public:
		World3D();
		~World3D();
	};
}

#endif // !_ISM_WORLD_3D_HPP_
