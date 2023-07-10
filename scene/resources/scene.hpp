#ifndef _ISM_SCENE_HPP_
#define _ISM_SCENE_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class SceneTree;

	class ISM_API Scene : public Resource
	{
		OBJECT_CLASS(Scene, Resource);

	public:
		Scene();
		~Scene();
	};
}

#endif // !_ISM_SCENE_HPP_
