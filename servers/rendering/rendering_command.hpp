#ifndef _ISM_RENDERING_COMMAND_HPP_
#define _ISM_RENDERING_COMMAND_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	template <class First, class ... Rest
	> static void render_immediate(RenderingDevice & device, First && first, Rest && ... rest)
	{
		mpl::for_args([&device](auto const & command) noexcept
		{
			std::invoke(command, &device);
		}
		, FWD(first), FWD(rest)...);
	}

	struct RenderingCommand : public std::function<void(RenderingDevice *)>
	{
	public:
		using base_type = typename std::function<void(RenderingDevice *)>;
		using base_type::function;
		using base_type::operator=;
		using base_type::operator();

	public:
		static RenderingCommand clear(Color const & color) noexcept
		{
			return std::bind(&RenderingDevice::clear, std::placeholders::_1, color);
		}

		static RenderingCommand draw_arrays(RenderPrimitive primitive, size_t first, size_t count) noexcept
		{
			return std::bind(&RenderingDevice::draw_arrays, std::placeholders::_1, primitive, first, count);
		}

		static RenderingCommand draw_indexed(RenderPrimitive primitive, size_t count) noexcept
		{
			return std::bind(&RenderingDevice::draw_indexed, std::placeholders::_1, primitive, count);
		}

		static RenderingCommand flush() noexcept
		{
			return std::bind(&RenderingDevice::flush, std::placeholders::_1);
		}

		static RenderingCommand set_viewport(Rect const & rect) noexcept
		{
			return std::bind(&RenderingDevice::set_viewport, std::placeholders::_1, rect);
		}
	};
}

#endif // !_ISM_RENDERING_COMMAND_HPP_
