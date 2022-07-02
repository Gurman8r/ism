#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/keyboard.hpp>
#include <core/input/input_event.hpp>
#include <core/input/input_map.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input action
	ALIAS(InputAction) int32_t;
	enum InputAction_ : InputAction
	{
		InputAction_Press,
		InputAction_Release,
		InputAction_Repeat,
	};

	// input mode
	ALIAS(InputMode) int32_t;
	enum InputMode_ : InputMode
	{
		InputMode_Cursor,
		InputMode_StickyKeys,
		InputMode_StickyMouseButtons,
		InputMode_LockKeyMods,
		InputMode_RawMouseMotion,
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

	// input state
	struct NODISCARD InputState final
	{
		char last_char;

		bool is_shift, is_ctrl, is_alt, is_super;

		Vec2 mouse_pos, mouse_delta;

		Vec2 scroll;

		BitSet<MouseButton_MAX> mouse_down;

		BitSet<KeyCode_MAX> keys_down;

		float_t mouse_down_duration[MouseButton_MAX];

		float_t keys_down_duration[KeyCode_MAX];
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input
	class ISM_API Input : public Object
	{
		OBJECT_COMMON(Input, Object);

		friend class Main;

		static Input * singleton;

		InputState m_state{};

	public:
		explicit Input() noexcept { singleton = this; }

		virtual ~Input() override {}

		FORCE_INLINE static Input * get_singleton() noexcept { return singleton; }

	public:
		NODISCARD InputState const & get_state() const noexcept { return m_state; }

		NODISCARD char get_char() const noexcept { return m_state.last_char; }

		NODISCARD bool is_shift() const noexcept { return m_state.is_shift; }
		NODISCARD bool is_ctrl() const noexcept { return m_state.is_ctrl; }
		NODISCARD bool is_alt() const noexcept { return m_state.is_alt; }
		NODISCARD bool is_super() const noexcept { return m_state.is_super; }

		NODISCARD bool is_shift(KeyCode_ value) const noexcept { return m_state.is_shift && m_state.keys_down.read(value); }
		NODISCARD bool is_ctrl(KeyCode_ value) const noexcept { return m_state.is_ctrl && m_state.keys_down.read(value); }
		NODISCARD bool is_alt(KeyCode_ value) const noexcept { return m_state.is_alt && m_state.keys_down.read(value); }
		NODISCARD bool is_super(KeyCode_ value) const noexcept { return m_state.is_super && m_state.keys_down.read(value); }

		NODISCARD bool get_key_up(KeyCode_ value) const noexcept { return !m_state.keys_down.read(value); }
		NODISCARD bool get_key_down(KeyCode_ value) const noexcept { return m_state.keys_down.read(value); }
		NODISCARD auto get_key_down_duration(KeyCode_ value) const noexcept { return m_state.keys_down_duration[value]; }

		NODISCARD auto get_mouse_pos() const noexcept -> Vec2 const & { return m_state.mouse_pos; }
		NODISCARD bool get_mouse_up(MouseButton_ value) const noexcept { return !m_state.mouse_down.read(value); }
		NODISCARD bool get_mouse_down(MouseButton_ value) const noexcept { return m_state.mouse_down.read(value); }
		NODISCARD auto get_mouse_down_duration(MouseButton_ value) const noexcept { return m_state.mouse_down_duration[value]; }
		NODISCARD auto get_mouse_delta() const noexcept -> Vec2 const & { return m_state.mouse_delta; }
		NODISCARD auto get_scroll() const noexcept -> Vec2 const & { return m_state.scroll; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INPUT (ism::Input::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
