#ifndef _ISM_SHADER_LANGUAGE_HPP_
#define _ISM_SHADER_LANGUAGE_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderLanguage : public Object
	{
		OBJECT_COMMON(ShaderLanguage, Object);

	protected:
		ShaderLanguage();

	public:
		virtual ~ShaderLanguage() override;

		virtual RD::ShaderCreateInfo generate_spec(JSON const & json) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SHADER_LANGUAGE_HPP_
