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
		template <class Fn, class ... Args
		> static auto bind(Fn && fn, Args && ... args) noexcept -> RenderingCommand {
			return std::bind(FWD(fn), std::placeholders::_1, FWD(args)...);
		}

	public:
		static auto clear(Color const & color = {}, bool depth_stencil = true) noexcept {
			return bind(&RenderingDevice::clear, color, depth_stencil);
		}

		static auto draw_arrays(RenderPrimitive primitive, size_t first, size_t count) noexcept {
			return bind(&RenderingDevice::draw_arrays, primitive, first, count);
		}

		static auto draw_indexed(RenderPrimitive primitive, size_t count) noexcept {
			return bind(&RenderingDevice::draw_indexed, primitive, count);
		}

		static auto flush() noexcept {
			return bind(&RenderingDevice::flush);
		}

		static auto set_viewport(IntRect const & rect) noexcept {
			return bind(&RenderingDevice::set_viewport, rect);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_COMMAND_HPP_
