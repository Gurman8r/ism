#ifndef _ISM_MATERIAL_HPP_
#define _ISM_MATERIAL_HPP_

#include <scene/resources/shader.hpp>
#include <scene/resources/texture.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Material : public Resource
	{
		OBJECT_CLASS(Material, Resource);

		friend class MaterialFormatLoader;

		RID m_material;

	protected:
		Material();

	public:
		virtual ~Material();
		virtual RID get_rid() const override { return m_material; }
		virtual RID get_shader_rid() const = 0;
		virtual RS::ShaderMode_ get_shader_mode() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderMaterial : public Material
	{
		OBJECT_CLASS(ShaderMaterial, Material);

		friend class MaterialFormatLoader;

		Ref<Shader> m_shader;

	public:
		ShaderMaterial();
		virtual ~ShaderMaterial() override;
		virtual RID get_shader_rid() const override;
		virtual RS::ShaderMode_ get_shader_mode() const override;

	public:
		Ref<Shader> get_shader() const;
		void set_shader(Ref<Shader> const & value);
		Var get_shader_param(String const & key) const;
		void set_shader_param(String const & key, Var const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API StandardMaterial3D : public Material
	{
		OBJECT_CLASS(StandardMaterial3D, Material);

		friend class MaterialFormatLoader;

	public:
		StandardMaterial3D();
		virtual ~StandardMaterial3D() override;
		virtual RID get_shader_rid() const override;
		virtual RS::ShaderMode_ get_shader_mode() const override;

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
