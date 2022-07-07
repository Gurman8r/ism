#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/keyboard.hpp>
#include <core/input/input_event.hpp>
#include <core/input/input_map.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input action
	enum InputAction_
	{
		InputAction_Press,
		InputAction_Release,
		InputAction_Repeat,
	};

	// mouse button
	ALIAS(MouseButton) int32_t;
	enum MouseButton_ : MouseButton
	{
		MouseButton_0,
		MouseButton_1,
		MouseButton_2,
		MouseButton_3,
		MouseButton_4,
		MouseButton_5,
		MouseButton_6,
		MouseButton_7,

		MouseButton_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input
	class ISM_API Input : public Object
	{
		OBJECT_COMMON(Input, Object);

		friend class Main;

		static Input * g_singleton;

		char m_last_char;
		bool m_is_shift : 1, m_is_ctrl : 1, m_is_alt : 1, m_is_super : 1;
		Vec2 m_mouse_pos{};
		Vec2 m_mouse_delta{};
		Vec2 m_scroll{};
		BitSet<MouseButton_MAX> m_mouse_down{};
		BitSet<KeyCode_MAX> m_keys_down{};
		Array<float_t, MouseButton_MAX> m_mouse_down_duration{};
		Array<float_t, KeyCode_MAX> m_keys_down_duration{};

	public:
		Input() noexcept { g_singleton = this; }
		virtual ~Input() noexcept override = default;
		FORCE_INLINE static Input * get_singleton() noexcept { return g_singleton; }

	public:
		char get_char() const noexcept { return m_last_char; }
		bool is_shift() const noexcept { return m_is_shift; }
		bool is_ctrl() const noexcept { return m_is_ctrl; }
		bool is_alt() const noexcept { return m_is_alt; }
		bool is_super() const noexcept { return m_is_super; }
		bool is_shift(KeyCode_ value) const noexcept { return m_is_shift && m_keys_down[value]; }
		bool is_ctrl(KeyCode_ value) const noexcept { return m_is_ctrl && m_keys_down[value]; }
		bool is_alt(KeyCode_ value) const noexcept { return m_is_alt && m_keys_down[value]; }
		bool is_super(KeyCode_ value) const noexcept { return m_is_super && m_keys_down[value]; }
		bool get_key_up(KeyCode_ value) const noexcept { return !m_keys_down[value]; }
		bool get_key_down(KeyCode_ value) const noexcept { return m_keys_down[value]; }
		auto get_key_down_duration(KeyCode_ value) const noexcept { return m_keys_down_duration[value]; }
		auto get_mouse_pos() const noexcept -> Vec2 const & { return m_mouse_pos; }
		bool get_mouse_up(MouseButton_ value) const noexcept { return !m_mouse_down[value]; }
		bool get_mouse_down(MouseButton_ value) const noexcept { return m_mouse_down[value]; }
		auto get_mouse_down_duration(MouseButton_ value) const noexcept { return m_mouse_down_duration[value]; }
		auto get_mouse_delta() const noexcept -> Vec2 const & { return m_mouse_delta; }
		auto get_scroll() const noexcept -> Vec2 const & { return m_scroll; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INPUT (ism::Input::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
