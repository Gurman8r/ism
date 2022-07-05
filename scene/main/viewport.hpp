#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <scene/resources/texture.hpp>

namespace ism
{
	class ViewportTexture : public Texture2D
	{
		OBJECT_COMMON(ViewportTexture, Texture2D);

	public:
		ViewportTexture();

		virtual ~ViewportTexture() override;

		virtual RID get_rid() const override;
		
		virtual int32_t get_width() const override;
		
		virtual int32_t get_height() const override;
		
		virtual Vec2i get_size() const override;
		
		virtual bool has_alpha() const override;
		
		virtual Ref<Image> get_image() const override;

	};

	// viewport
	class ISM_API Viewport : public Node
	{
		OBJECT_COMMON(Viewport, Node);

	protected:
		Viewport();

	public:
		virtual ~Viewport() override;

	public:
		virtual Vec2i get_position() const = 0;
		virtual Vec2i get_size() const = 0;
		IntRect get_rect() const noexcept { return { get_position(), get_size() }; }

		virtual void set_position(Vec2i const & value) = 0;
		virtual void set_size(Vec2i const & value) = 0;
		void set_rect(IntRect const & value) noexcept { set_position({ value[0], value[1] }); set_size({ value[2], value[3] }); }
	};
}

#endif // !_ISM_VIEWPORT_HPP_
