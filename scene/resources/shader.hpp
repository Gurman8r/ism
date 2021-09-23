#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Shader : public Resource
	{
		OBJ_COMMON(Shader, Resource);

	public:
		virtual ~Shader();

		Shader();

	private:

	};
}

#endif // !_ISM_SHADER_HPP_
