#ifndef _ISM_TEXTURE_HPP_
#define _ISM_TEXTURE_HPP_

#include <core/io/image.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture : public Resource
	{
		OBJECT_COMMON(Texture, Resource);

	public:
		Texture() noexcept {}
		virtual ~Texture() noexcept override = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture2D : public Texture
	{
		OBJECT_COMMON(Texture2D, Texture);

	public:
		Texture2D() noexcept {}
		virtual ~Texture2D() noexcept override = default;

		NODISCARD virtual int32_t get_width() const = 0;
		NODISCARD virtual int32_t get_height() const = 0;
		NODISCARD virtual Ref<Image> get_data() const { return nullptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ImageTexture : public Texture2D
	{
		OBJECT_COMMON(ImageTexture, Texture2D);

		mutable RID		m_texture{};
		ImageFormat_	m_format{};
		int32_t			m_width{};
		int32_t			m_height{};
		bool			m_has_mipmaps{};
		bool			m_image_stored{};

	public:
		ImageTexture(Ref<Image> const & image);
		virtual ~ImageTexture() override;

		NODISCARD virtual int32_t get_width() const override { return m_width; }
		NODISCARD virtual int32_t get_height() const override { return m_height; }
		NODISCARD virtual Ref<Image> get_data() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture3D : public Texture
	{
		OBJECT_COMMON(Texture3D, Texture);

	public:
		Texture3D();
		virtual ~Texture3D() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API TextureCube : public Texture
	{
		OBJECT_COMMON(TextureCube, Texture);

	public:
		TextureCube();
		virtual ~TextureCube() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TEXTURE_HPP_
