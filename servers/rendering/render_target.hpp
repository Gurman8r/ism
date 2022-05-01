#ifndef _ISM_RENDER_TARGET_HPP_
#define _ISM_RENDER_TARGET_HPP_

#include <scene/resources/texture.hpp>
#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// framebuffer
	class ISM_API Framebuffer : public Resource
	{
		OBJECT_COMMON(Framebuffer, Resource);

		RID m_framebuffer{};

	public:
		explicit Framebuffer(RID framebuffer) noexcept : m_framebuffer{ framebuffer } {};

		Framebuffer(FramebufferSpecification const & spec = { 1280, 720, { ColorFormat_R8G8B8_UNORM, ColorFormat_D24_UNORM_S8_UINT } });

		Framebuffer(Framebuffer && other) noexcept { swap(std::move(other)); }
		
		Framebuffer & operator=(Framebuffer && other) noexcept { return swap(std::move(other)); }
		
		Framebuffer & swap(Framebuffer & other) noexcept { if (this != std::addressof(other)) { std::swap(m_framebuffer, other.m_framebuffer); } return (*this); }

		virtual ~Framebuffer() override;

	public:
		void bind();

		void unbind();

		void resize(int32_t width, int32_t size);

	public:
		NODISCARD RID get_attachment(size_t index = 0) const noexcept { ASSERT(index < get_attachment_count()); return get_attachment_data()[index]; }

		NODISCARD virtual size_t get_attachment_count() const;

		NODISCARD virtual RID * get_attachment_data() const;

		NODISCARD virtual int32_t get_width() const;

		NODISCARD virtual int32_t get_height() const;

		NODISCARD virtual RID get_rid() const override { return m_framebuffer; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDER_TARGET_HPP_
