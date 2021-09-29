#ifndef _ISM_TEXTURE_HPP_
#define _ISM_TEXTURE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture : public Resource
	{
		OBJECT_COMMON(Texture, Resource);

	public:
		Texture();

		virtual ~Texture();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture2D : public Texture
	{
		OBJECT_COMMON(Texture2D, Texture);

	public:
		Texture2D();

		virtual ~Texture2D();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture3D : public Texture
	{
		OBJECT_COMMON(Texture3D, Texture);

	public:
		Texture3D();

		virtual ~Texture3D();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API TextureCube : public Texture
	{
		OBJECT_COMMON(TextureCube, Texture);

	public:
		TextureCube();

		virtual ~TextureCube();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TEXTURE_HPP_
