#ifndef _ISM_TEXTURE_HPP_
#define _ISM_TEXTURE_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture : public Resource
	{
		OBJECT_COMMON(Texture, Resource);

	protected:
		Texture() noexcept {}

	public:
		virtual ~Texture() noexcept override = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Texture2D : public Texture
	{
		OBJECT_COMMON(Texture2D, Texture);

	protected:
		Texture2D() noexcept {}

	public:
		virtual ~Texture2D() noexcept override = default;

		virtual int32_t get_width() const = 0;
		
		virtual int32_t get_height() const = 0;
		
		virtual Ref<Image> get_image() const { return nullptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ImageTexture : public Texture2D
	{
		OBJECT_COMMON(ImageTexture, Texture2D);

		RID			m_texture{};
		int32_t		m_width{};
		int32_t		m_height{};
		Ref<Image>	m_image{};

	public:
		ImageTexture() noexcept {}
		
		ImageTexture(Path const & path);
		
		ImageTexture(Ref<Image> const & image);
		
		virtual ~ImageTexture() override;

		virtual RID get_rid() const override { return m_texture; }
		
		virtual int32_t get_width() const override { return m_width; }
		
		virtual int32_t get_height() const override { return m_height; }
		
		virtual Ref<Image> get_image() const override;

		void update(Ref<Image> const & image, bool immediate = false);
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
