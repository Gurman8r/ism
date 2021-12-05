#ifndef _ISM_RENDERER_STORAGE_HPP_
#define _ISM_RENDERER_STORAGE_HPP_

#include <servers/rendering/rendering_device.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// rendering device structure
#define RD_STRUCT(m_struct)													\
public:																		\
	DEFAULT_COPYABLE_MOVABLE(m_struct);										\
																			\
	NODISCARD operator RID() const noexcept { return (RID)(void *)this; }	\
																			\
	NODISCARD bool operator==(m_struct const & other) const noexcept {		\
		return this == std::addressof(other);								\
	}																		\
public:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	struct NODISCARD NOVTABLE RD_Base
	{
	protected:
		DEFAULT_COPYABLE_MOVABLE(RD_Base);

	public:
		RID handle{};
	};

	struct NODISCARD RD_VertexArray final : RD_Base
	{
		RD_STRUCT(RD_VertexArray);
	};

	struct NODISCARD RD_VertexBuffer final : RD_Base
	{
		RD_STRUCT(RD_VertexBuffer);
	};

	struct NODISCARD RD_IndexBuffer final : RD_Base
	{
		RD_STRUCT(RD_IndexBuffer);
	};

	struct NODISCARD RD_Texture final : RD_Base
	{
		RD_STRUCT(RD_Texture);

		TextureType_ texture_type{};
		
		DataFormat_ data_format{}, data_format_srgb{};
		
		TextureView view{};
		
		ImageFormat_ image_format{};

		int32_t width{}, height{}, depth{}, layers{}, mipmaps{};

		int32_t width_2d{ width }, height_2d{ height }; // size override

		TextureFlags usage_flags{};
	};

	struct NODISCARD RD_FrameBuffer final : RD_Base
	{
		RD_STRUCT(RD_FrameBuffer);

		Vector<RID> textures{};

		Vec2i size{};
	};

	struct NODISCARD RD_Shader final : RD_Base
	{
		RD_STRUCT(RD_Shader);
	};
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
