#ifndef _ISM_MATERIAL_HPP_
#define _ISM_MATERIAL_HPP_

#include <scene/resources/shader.hpp>
#include <scene/resources/texture.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Material : public Resource
	{
		OBJECT_COMMON(Material, Resource);

		RID m_material;

	protected:
		Material();

	public:
		virtual ~Material();

		virtual Error_ reload_from_file() override = 0;

		virtual RID get_rid() const override { return m_material; }

		virtual RID get_shader_rid() const = 0;

		virtual Shader::Mode_ get_shader_mode() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderMaterial : public Material
	{
		OBJECT_COMMON(ShaderMaterial, Material);

		Ref<Shader> m_shader;

	public:
		ShaderMaterial();

		explicit ShaderMaterial(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~ShaderMaterial() override;

		virtual Error_ reload_from_file() override;

		virtual RID get_shader_rid() const override;

		virtual Shader::Mode_ get_shader_mode() const override;

	public:
		Ref<Shader> get_shader() const;
		void set_shader(Ref<Shader> const & value);

		Variant get_shader_param(StringName const & key) const;
		void set_shader_param(StringName const & key, Variant const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API BaseMaterial3D : public Material
	{
		OBJECT_COMMON(BaseMaterial3D, Material);

	public:
		enum Param_
		{
			Param_Albedo,
			Param_Specular,
			Param_Metallic,
			// etc...
			Param_MAX
		};

		static constexpr cstring parameter_names[Param_MAX]
		{
			"Albedo",
			"Specular",
			"Metallic",
			// etc...
		};

	public:
		BaseMaterial3D();

		explicit BaseMaterial3D(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~BaseMaterial3D() override;

		virtual Error_ reload_from_file() override;

		virtual RID get_shader_rid() const override;

		virtual Shader::Mode_ get_shader_mode() const override;

	private:
		void _update_shader();

		struct Params
		{
			Color albedo{};
			f32 specular{};
			f32 metallic{};
			// etc...
		}
		m_params{};

	public:
		Color get_albedo() const { return m_params.albedo; }
		void set_albedo(Color const & value);

		f32 get_specular() const { return m_params.specular; }
		void set_specular(f32 value);

		f32 get_metallic() const { return m_params.metallic; }
		void set_metallic(f32 value);

		// etc...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATERIAL_HPP_
