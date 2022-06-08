#ifndef _ISM_SHADER_LANGUAGE_HPP_
#define _ISM_SHADER_LANGUAGE_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderBuilder
	{
	public:
		virtual ~ShaderBuilder() noexcept = default;

	public:
		enum Keyword_
		{
			Keyword_INVALID = -1,
			Keyword_Position,
			Keyword_PointSize,
			Keyword_ClipDistance,
			Keyword_Texture,
			Keyword_MAX
		};

		enum Type_
		{
			Type_INVALID = -1,
			Type_Void,
			Type_Bool,
			Type_Int,
			Type_Float,
			Type_Vec2,
			Type_Vec3,
			Type_Vec4,
			Type_Mat2,
			Type_Mat3,
			Type_Mat4,
			Type_Texture2D,
			Type_Texture3D,
			Type_Cubemap,
			Type_MAX
		};

		virtual String get_keyword(Keyword_ keyword) const = 0;
		virtual String get_type_name(Type_ type) const = 0;

	public:
		struct VarDef
		{
			String name{};
			int32_t binding{ -1 };
			Type_ type{ Type_MAX };
		};

		struct BufferDef
		{
			String name{};
			int32_t binding{ -1 };
			Vector<VarDef> data{};
		};

		struct FunctionDef
		{
			String name{};
			Type_ out_type{ Type_MAX };
			Vector<Type_> in_types{};
		};

		struct MainDef : FunctionDef
		{
		};

		struct StageDef
		{
			RD::ShaderStage_	shader_stage{};
			Vector<VarDef>		inputs{};
			Vector<VarDef>		outputs{};
			Vector<BufferDef>	buffers{};
			Vector<FunctionDef>	functions{};
			MainDef				main{};
		};

	public:
		ShaderBuilder & uniform(int32_t binding, Type_ type, String const & name) { return (*this); }
		ShaderBuilder & uniform_bool(int32_t binding, String const & name) { return uniform(binding, Type_Bool, name); }
		ShaderBuilder & uniform_int(int32_t binding, String const & name) { return uniform(binding, Type_Int, name); }
		ShaderBuilder & uniform_float(int32_t binding, String const & name) { return uniform(binding, Type_Float, name); }
		ShaderBuilder & uniform_vec2(int32_t binding, String const & name) { return uniform(binding, Type_Vec2, name); }
		ShaderBuilder & uniform_vec3(int32_t binding, String const & name) { return uniform(binding, Type_Vec3, name); }
		ShaderBuilder & uniform_vec4(int32_t binding, String const & name) { return uniform(binding, Type_Vec4, name); }
		ShaderBuilder & uniform_mat2(int32_t binding, String const & name) { return uniform(binding, Type_Mat2, name); }
		ShaderBuilder & uniform_mat3(int32_t binding, String const & name) { return uniform(binding, Type_Mat3, name); }
		ShaderBuilder & uniform_mat4(int32_t binding, String const & name) { return uniform(binding, Type_Mat4, name); }
		ShaderBuilder & uniform_texture2D(int32_t binding, String const & name) { return uniform(binding, Type_Texture2D, name); }
		ShaderBuilder & uniform_texture3D(int32_t binding, String const & name) { return uniform(binding, Type_Texture3D, name); }
		ShaderBuilder & uniform_cubemap(int32_t binding, String const & name) { return uniform(binding, Type_Cubemap, name); }


	protected:
		virtual void do_input(DynamicBuffer & code, VarDef const & value) const = 0;
		virtual void do_output(DynamicBuffer & code, VarDef const & value) const = 0;
		virtual void do_uniform(DynamicBuffer & code, VarDef const & value) const = 0;

	private:
		struct Head {
			StageDef * stage{};
			BufferDef * buffer{};
			MainDef * main{};
		} m_head{};

		Vector<StageDef> m_stages{};

		DynamicBuffer m_attributes{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderBuilderGLSL : public ShaderBuilder
	{
	public:
		ShaderBuilderGLSL() noexcept {}

		virtual ~ShaderBuilderGLSL() override {}

	public:
		static constexpr Array<StringView, Keyword_MAX> _keyword_sv
		{
			"gl_Position"_sv,
			"gl_PointSize"_sv,
			"gl_ClipDistance"_sv,
			"texture"_sv,
		};

		static constexpr Array<StringView, Type_MAX> _type_sv
		{
			"void"_sv,
			"int"_sv,
			"int"_sv,
			"float"_sv,
			"vec2"_sv,
			"vec3"_sv,
			"vec4"_sv,
			"mat2"_sv,
			"mat3"_sv,
			"mat4"_sv,
			"sampler2D"_sv,
			"sampler3D"_sv,
			"samplerCube"_sv,
		};

		virtual String get_keyword(Keyword_ keyword) const override {
			ASSERT(Keyword_INVALID < keyword && keyword < Keyword_MAX);
			return _keyword_sv[(size_t)keyword];
		}

		virtual String get_type_name(Type_ type) const override {
			ASSERT(Type_INVALID < type && type < Type_MAX);
			return _type_sv[(size_t)type];
		}

	protected:
		virtual void do_input(DynamicBuffer & code, VarDef const & value) const override {
			StringView const & type_name{ _type_sv[value.type] };
			code.printf("layout (location = %i) in %.*s %.*s;\n",
				value.binding,
				type_name.size(), type_name.data(),
				value.name.size(), value.name.data());
		}

		virtual void do_output(DynamicBuffer & code, VarDef const & value) const override {
			StringView const & type_name{ _type_sv[value.type] };
			code.printf("layout (location = %i) code %.*s %.*s;\n",
				value.binding,
				type_name.size(), type_name.data(),
				value.name.size(), value.name.data());
		}

		virtual void do_uniform(DynamicBuffer & code, VarDef const & value) const override {
			StringView const & type_name{ _type_sv[value.type] };
			code.printf("layout (location = %i) uniform %.*s %.*s;\n",
				value.binding,
				type_name.size(), type_name.data(),
				value.name.size(), value.name.data());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SHADER_LANGUAGE_HPP_
