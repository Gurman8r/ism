#ifndef _ISM_WORLD_2D_HPP_
#define _ISM_WORLD_2D_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API World2D : public Resource
	{
		OBJECT_CLASS(World2D, Resource);

	public:
		World2D();
		~World2D();
	};
}

#endif // !_ISM_WORLD_2D_HPP_
