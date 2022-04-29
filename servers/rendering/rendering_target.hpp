#ifndef _ISM_RENDERING_TARGET_HPP_
#define _ISM_RENDERING_TARGET_HPP_

#include <scene/resources/texture.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// rendering texture
namespace ism
{
	class ISM_API RenderingTexture : public Texture2D
	{
		OBJECT_COMMON(RenderingTexture, Texture2D);

	public:
		virtual ~RenderingTexture();
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// rendering target
namespace ism
{
	class ISM_API RenderingTarget : public Object
	{
		OBJECT_COMMON(RenderingTarget, Object);

	public:
		virtual ~RenderingTarget();
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_RENDERING_TARGET_HPP_
