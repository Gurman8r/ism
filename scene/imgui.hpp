#ifndef _ISM_IMGUI_HPP_
#define _ISM_IMGUI_HPP_

#include <scene/main/window.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef IM_VEC2_CLASS_EXTRA
#define IM_VEC2_CLASS_EXTRA																		\
																								\
	constexpr ImVec2(ism::Vec2 const & v) noexcept : x{ v[0] }, y{ v[1] } {}					\
																								\
	constexpr operator ism::Vec2() const noexcept { return *(ism::Vec2 *)&x; }					\
																								\
	ImVec2 operator-(ImVec2 const & v) const noexcept { return ImVec2{ x - v.x, y - v.y }; }	\
																								\
	friend ImVec2 & operator+=(ImVec2 & a, ism::Vec2 const & b) noexcept {						\
		a.x += b[0]; a.y += b[1];																\
		return a;																				\
	}																							\
	friend ImVec2 & operator-=(ImVec2 & a, ism::Vec2 const & b) noexcept {						\
		a.x -= b[0]; a.y -= b[1];																\
		return a;																				\
	}																							\
	friend ImVec2 & operator*=(ImVec2 & a, ism::Vec2 const & b) noexcept {						\
		a.x *= b[0]; a.y *= b[1];																\
		return a;																				\
	}																							\
	friend ImVec2 & operator/=(ImVec2 & a, ism::Vec2 const & b) noexcept {						\
		a.x /= b[0]; a.y /= b[1];																\
		return a;																				\
	}																							\
																								\
	friend ImVec2 operator+(ImVec2 & a, ism::Vec2 const & b) noexcept {							\
		ImVec2 temp{ a };																		\
		return temp += b;																		\
	}																							\
	friend ImVec2 operator-(ImVec2 & a, ism::Vec2 const & b) noexcept {							\
		ImVec2 temp{ a };																		\
		return temp -= b;																		\
	}																							\
	friend ImVec2 operator*(ImVec2 & a, ism::Vec2 const & b) noexcept {							\
		ImVec2 temp{ a };																		\
		return temp *= b;																		\
	}																							\
	friend ImVec2 operator/(ImVec2 & a, ism::Vec2 const & b) noexcept {							\
		ImVec2 temp{ a };																		\
		return temp /= b;																		\
	}																							\
																								\

#endif

#ifndef IM_VEC4_CLASS_EXTRA
#define IM_VEC4_CLASS_EXTRA																			\
																									\
	constexpr ImVec4(ism::Vec4 const & v) noexcept : x{ v[0] }, y{ v[1] }, z{ v[2] }, w{ v[3] } {}	\
																									\
	constexpr ImVec4(ism::Color const & c) noexcept : x{ c[0] }, y{ c[1] }, z{ c[2] }, w{ c[3] } {}	\
																									\
	constexpr operator ism::Vec4() const noexcept { return *(ism::Vec4 *)&x; }						\
																									\
	constexpr operator ism::Color() const noexcept { return *(ism::Color *)&x; }					\
																									\
	friend ImVec4 & operator+=(ImVec4 & a, ism::Vec4 const & b) noexcept {							\
		a.x += b[0]; a.y += b[1]; a.z += b[2]; a.w += b[3];											\
		return a;																					\
	}																								\
	friend ImVec4 & operator-=(ImVec4 & a, ism::Vec4 const & b) noexcept {							\
		a.x -= b[0]; a.y -= b[1]; a.z -= b[2]; a.w -= b[3];											\
		return a;																					\
	}																								\
	friend ImVec4 & operator*=(ImVec4 & a, ism::Vec4 const & b) noexcept {							\
		a.x *= b[0]; a.y *= b[1]; a.z *= b[2]; a.w *= b[3];											\
		return a;																					\
	}																								\
	friend ImVec4 & operator/=(ImVec4 & a, ism::Vec4 const & b) noexcept {							\
		a.x /= b[0]; a.y /= b[1]; a.z /= b[2]; a.w /= b[3];											\
		return a;																					\
	}																								\
																									\
	friend ImVec4 operator+(ImVec4 & a, ism::Vec4 const & b) noexcept {								\
		ImVec4 temp{ a };																			\
		return temp += b;																			\
	}																								\
	friend ImVec4 operator-(ImVec4 & a, ism::Vec4 const & b) noexcept {								\
		ImVec4 temp{ a };																			\
		return temp -= b;																			\
	}																								\
	friend ImVec4 operator*(ImVec4 & a, ism::Vec4 const & b) noexcept {								\
		ImVec4 temp{ a };																			\
		return temp *= b;																			\
	}																								\
	friend ImVec4 operator/(ImVec4 & a, ism::Vec4 const & b) noexcept {								\
		ImVec4 temp{ a };																			\
		return temp /= b;																			\
	}																								\
																									\

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>
#include <ImGuizmo/ImGuizmo.h>

#if 0 // extras
#include <ImGuizmo/ImSequencer.h>
#include <ImGuizmo/ImCurveEdit.h>
#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui-node-editor/imgui_node_editor.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	ISM_API_FUNC(bool) ImGui_Init(WindowID window, bool install_callbacks = true);

	ISM_API_FUNC(void) ImGui_Shutdown();

	ISM_API_FUNC(void) ImGui_NewFrame();

	ISM_API_FUNC(void) ImGui_RenderDrawData(ImDrawData * draw_data);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_IMGUI_HPP_
