#ifndef _ISM_RENDERING_COMMAND_HPP_
#define _ISM_RENDERING_COMMAND_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class First, class ... Rest
	> static void render_immediate(RenderingDevice * device, First && first, Rest && ... rest)
	{
		VERIFY(device);

		mpl::for_args([device](auto && command) noexcept
		{
			std::invoke(FWD(command), device);
		}
		, FWD(first), FWD(rest)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct RenderingCommand : public std::function<void(RenderingDevice *)>
	{
	public:
		using base_type = typename std::function<void(RenderingDevice *)>;
		using base_type::function;
		using base_type::operator=;
		using base_type::operator();

	public:
		template <class First, class ... Rest
		> static auto render_immediate(First && first, Rest && ... rest) -> RenderingCommand {
			return std::bind(ism::render_immediate, std::placeholders::_1, FWD(first), FWD(rest)...);
		}

	public:
		static auto clear(Color const & color, bool depth_stencil = true) noexcept -> RenderingCommand {
			return std::bind(&RenderingDevice::clear, std::placeholders::_1, color, depth_stencil);
		}

		static auto draw_arrays(RenderPrimitive primitive, size_t first, size_t count) noexcept -> RenderingCommand {
			return std::bind(&RenderingDevice::draw_arrays, std::placeholders::_1, primitive, first, count);
		}

		static auto draw_indexed(RenderPrimitive primitive, size_t count) noexcept -> RenderingCommand {
			return std::bind(&RenderingDevice::draw_indexed, std::placeholders::_1, primitive, count);
		}

		static auto flush() noexcept -> RenderingCommand {
			return std::bind(&RenderingDevice::flush, std::placeholders::_1);
		}

		static auto set_viewport(Rect const & rect) noexcept -> RenderingCommand {
			return std::bind(&RenderingDevice::set_viewport, std::placeholders::_1, rect);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_COMMAND_HPP_
