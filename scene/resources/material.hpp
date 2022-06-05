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

		NODISCARD virtual RID get_rid() const override { return m_material; }

		NODISCARD virtual RID get_shader_rid() const = 0;

		NODISCARD virtual Shader::Mode_ get_shader_mode() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderMaterial : public Material
	{
		OBJECT_COMMON(ShaderMaterial, Material);

		Ref<Shader> m_shader;

	public:
		explicit ShaderMaterial();

		explicit ShaderMaterial(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~ShaderMaterial() override;

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_shader_rid() const override;

		NODISCARD virtual Shader::Mode_ get_shader_mode() const override;

	public:
		NODISCARD Ref<Shader> get_shader() const;
		void set_shader(Ref<Shader> const & value);

		NODISCARD OBJ get_shader_param(String const & key) const;
		void set_shader_param(String const & key, OBJ const & value);
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
			Param_MAX
		};

		static constexpr cstring parameter_names[Param_MAX]
		{
			"Albedo",
			"Specular",
			"Metallic",
		};

	public:
		explicit BaseMaterial3D();

		explicit BaseMaterial3D(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~BaseMaterial3D() override;

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_shader_rid() const override;

		NODISCARD virtual Shader::Mode_ get_shader_mode() const override;

	private:
		void _update_shader();

		struct Params
		{
			Color albedo{};
			float_t specular{};
			float_t metallic{};
		}
		m_params{};

	public:
		NODISCARD Color get_albedo() const { return m_params.albedo; }
		void set_albedo(Color const & value);

		NODISCARD float_t get_specular() const { return m_params.specular; }
		void set_specular(float_t value);

		NODISCARD float_t get_metallic() const { return m_params.metallic; }
		void set_metallic(float_t value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATERIAL_HPP_
