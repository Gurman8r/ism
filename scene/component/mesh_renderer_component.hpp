#ifndef _ISM_MESH_RENDERER_COMPONENT_HPP_
#define _ISM_MESH_RENDERER_COMPONENT_HPP_

#include <scene/component/renderer_component.hpp>

namespace ism
{
	class ISM_API MeshRendererComponent : public RendererComponent
	{
		DEFINE_CLASS(MeshRendererComponent, RendererComponent);

	public:
		MeshRendererComponent();
		virtual ~MeshRendererComponent() override;
	};
}

#endif // !_ISM_MESH_RENDERER_COMPONENT_HPP_
