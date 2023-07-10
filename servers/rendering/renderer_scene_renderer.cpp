#include <servers/rendering/renderer_scene_renderer.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RendererSceneRenderer::RendererSceneRenderer(RenderingDevice * device, RendererStorage * storage)
		: m_device{ VALIDATE(device) }
		, m_storage{ VALIDATE(storage) }
	{
	}

	RendererSceneRenderer::~RendererSceneRenderer()
	{
	}

	void RendererSceneRenderer::initialize()
	{

	}

	void RendererSceneRenderer::finalize()
	{

	}

	void RendererSceneRenderer::update_shader(RID shader)
	{
		if (m_shader == shader) {
			return;
		}
		else if (m_shader && !shader) {
			m_shader = nullptr;
			return;
		}
		else {
			m_shader = shader;
		}

		RD::RasterizationState r{};
		r.enable_depth_clamp = false;
		r.discard_primitives = false;
		r.enable_wireframe = false;
		r.cull_mode = RD::PolygonCullMode_Front;
		r.front_face = RD::PolygonFrontFace_Clockwise;
		r.enable_depth_bias = false;
		r.depth_bias_constant_factor = 0.f;
		r.depth_bias_clamp = 0.f;
		r.depth_bias_slope_factor = 0.f;
		r.line_width = 1.f;
		r.patch_control_points = 1;

		RD::MultisampleState m{};
		m.sample_count = RD::TextureSamples_1;
		m.enable_sample_shading = false;
		m.sample_mask = {};
		m.enable_alpha_to_coverage = false;
		m.enable_alpha_to_one = false;

		RD::DepthStencilState d{};
		d.enable_depth_test = true;
		d.enable_depth_write = true;
		d.depth_compare_operator = CompareOperator_Less;
		d.enable_depth_range = false;
		d.depth_range_min = 0.f;
		d.depth_range_max = 0.f;
		d.enable_stencil = false;
		d.front_op.fail = d.back_op.fail = RD::StencilOperation_Zero;
		d.front_op.pass = d.back_op.pass = RD::StencilOperation_Zero;
		d.front_op.depth_fail = d.back_op.depth_fail = RD::StencilOperation_Zero;
		d.front_op.compare = d.back_op.compare = CompareOperator_Always;
		d.front_op.compare_mask = d.back_op.compare_mask = 0;
		d.front_op.write_mask = d.back_op.write_mask = 0;
		d.front_op.reference = d.back_op.reference = 0;

		RD::ColorBlendState c{};
		c.enable_logic_op = true;
		c.logic_op = LogicOperation_Clear;
		c.blend_constant = Colors::clear;
		c.attachments.push_back({ true,
			RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
			RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
			true, true, true, true });

		m_pipeline = m_device->render_pipeline_create(m_shader, RD::RenderPrimitive_Triangles, r, m, d, c);

		ASSERT(m_pipeline);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
