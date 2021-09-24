#ifndef _ISM_TEXTURE_HPP_
#define _ISM_TEXTURE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Texture : public Resource
	{
		OBJECT_CLASS(Texture, Resource);

	public:
		virtual ~Texture();

		Texture();

	private:

	};
}

#endif // !_ISM_TEXTURE_HPP_
