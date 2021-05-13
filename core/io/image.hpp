#ifndef _ISM_IMAGE_HPP_
#define _ISM_IMAGE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	ALIAS(Color32) uint32_t;

	class ISM_API Image : public Resource
	{
		ISM_SUPER(Image, Resource);

	private:
		friend class ImageLoader;

		int32_t m_width{}, m_height{}, m_channels{};
		
		Vector<byte> m_pixels{};

	public:
		void flip_vertically();

		void flip_horizontally();

		int32_t get_width() const;
		
		int32_t get_height() const;
		
		int32_t get_channels() const;

		Vector<byte> const & get_pixels() const;

		Color32 get_pixel(size_t i) const;

		Color32 get_pixel(size_t x, size_t y) const;

		void set_pixel(size_t i, Color32 value);

		void set_pixel(size_t x, size_t y, Color32 value);

		Image() {}

		virtual ~Image() override;

	};
}

#endif // !_ISM_IMAGE_HPP_
