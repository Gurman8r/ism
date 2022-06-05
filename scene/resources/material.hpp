#ifndef _ISM_MATERIAL_HPP_
#define _ISM_MATERIAL_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Material : public Resource
	{
		OBJECT_COMMON(Material, Resource);

	protected:
		RID m_material;

		Material();

	public:
		virtual ~Material();

		NODISCARD virtual RID get_rid() const { return m_material; }

		NODISCARD virtual RID get_shader_rid() const = 0;

		virtual Error_ reload_from_file() override = 0;
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
		BaseMaterial3D();

		explicit BaseMaterial3D(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~BaseMaterial3D();

		NODISCARD virtual RID get_shader_rid() const override;

		virtual Error_ reload_from_file() override;

	private:
		void _update_shader();

		struct Params final
		{
			Color albedo{};
			float_t specular{};
			float_t metallic{};
		}
		m_params{};

	public:
		Color get_albedo() const { return m_params.albedo; }
		float_t get_specular() const { return m_params.specular; }
		float_t get_metallic() const { return m_params.metallic; }

		void set_albedo(Color const & value);
		void set_specular(float_t value);
		void set_metallic(float_t value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATERIAL_HPP_
