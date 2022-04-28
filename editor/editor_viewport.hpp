#ifndef _ISM_EDITOR_VIEWPORT_HPP_
#define _ISM_EDITOR_VIEWPORT_HPP_

#include <editor/editor_panel.hpp>

namespace ism
{
	class ISM_API EditorViewport : public EditorPanel
	{
		OBJECT_COMMON(EditorViewport, EditorPanel);

		RID m_main_texture;
		Vec2f m_uv[2];
		Color32 m_tint;

	public:
		static constexpr Vec2 default_uvs[2] = { { 0, 1 }, { 1, 0 } };
		static constexpr Color32 default_tint{ 0xffffffff };

		EditorViewport(Vec2f const (&uvs)[2] = default_uvs, Color32 const & tint = default_tint);

		virtual ~EditorViewport();

		auto get_main_texture() const -> RID { return m_main_texture; }
		void set_main_texture(RID value) { m_main_texture = value; }

		auto get_uv(size_t const i) const noexcept -> Vec2f { ASSERT(i < 2); return m_uv[i]; }
		auto get_uv0() const noexcept -> Vec2 { return m_uv[0]; }
		auto get_uv1() const noexcept -> Vec2 { return m_uv[1]; }
		auto get_uvs() const noexcept -> Vec2f const * { return m_uv; }
		void set_uvs(Vec2f const & uv0, Vec2f const & uv1) noexcept { m_uv[0] = uv0; m_uv[1] = uv1; }
		void set_uvs(Vec2f const (&uvs)[2]) noexcept { m_uv[0] = uvs[0]; m_uv[1] = uvs[1]; }

		auto get_tint() const -> Color32 { return m_tint; }
		void set_tint(Color32 const & value) { m_tint = value; }

	public:
		virtual void draw() override;
	};
}

#endif // !_ISM_EDITOR_VIEWPORT_HPP_
