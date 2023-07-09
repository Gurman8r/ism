#ifndef _ISM_SHADER_LIBRARY_HPP_
#define _ISM_SHADER_LIBRARY_HPP_

#include <scene/resources/shader.hpp>
#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ShaderFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(ShaderFormatLoader, ResourceFormatLoader);
	public:
		static Error_ load_shader(Ref<Shader> shader, String const & path);
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SHADER_LIBRARY_HPP_
