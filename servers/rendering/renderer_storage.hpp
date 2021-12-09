#ifndef _ISM_RENDERER_STORAGE_HPP_
#define _ISM_RENDERER_STORAGE_HPP_VertexArray

#include <servers/rendering/rendering_device.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	struct NODISCARD RD_Vertexarray final
	{
		DEFAULT_COPYABLE_MOVABLE(RD_Vertexarray);

		RID handle{};

		RenderPrimitive_ primitive{};

		RID indices{};

		Vector<RID> vertices{};
	};

	struct NODISCARD RD_Vertexbuffer final
	{
		DEFAULT_COPYABLE_MOVABLE(RD_Vertexbuffer);

		RID handle{};

		Buffer buffer{};
	};

	struct NODISCARD RD_Indexbuffer final
	{
		DEFAULT_COPYABLE_MOVABLE(RD_Indexbuffer);

		RID handle{};

		Buffer buffer{};
	};

	struct NODISCARD RD_Texture final
	{
		DEFAULT_COPYABLE_MOVABLE(RD_Texture);

		RID handle{};

		TextureType_ texture_type{};
		
		ColorFormat_ color_format{}, color_format_srgb{};
		
		ImageFormat_ image_format{};
		
		int32_t width{}, height{}, depth{}, layers{}, mipmaps{};
		
		SamplerFilter_ min_filter{}, mag_filter{};

		SamplerRepeatMode_ repeat_s{}, repeat_t{};

		TextureSamples_ samples{};
		
		TextureSwizzle_ swizzle_r{}, swizzle_g{}, swizzle_b{}, swizzle_a{};

		TextureFlags usage_flags{};

		int32_t width_2d{ width }, height_2d{ height }; // size override
	};

	struct NODISCARD RD_Framebuffer final
	{
		DEFAULT_COPYABLE_MOVABLE(RD_Framebuffer);

		RID handle{};

		int32_t width{}, height{};
		
		Vector<RID> texture_attachments{};
	};

	struct NODISCARD RD_Shader final
	{
		DEFAULT_COPYABLE_MOVABLE(RD_Shader);

		RID handle{};

		FlatMap<hash_t, int32_t> bindings{};

		Vector<ShaderStageData> stage_data{};
	};
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
