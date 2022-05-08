#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum {
	SCENE,
	RENDER_PASS,
	TRANSFORMS,
	MATERIAL,
	MAX_UNIFORMS
};

static RID uniform_buffers[MAX_UNIFORMS]{};
static RID uniform_sets[MAX_UNIFORMS]{};
static RID pipeline{};
static RID framebuffer{}, backbuffer{};
static RID render_target{};
static RID viewport{};

static void _setup_pipeline(RID const shader)
{
	pipeline = RENDERING_DEVICE->pipeline_create(
		shader,
		RD::RenderPrimitive_Triangles,
		COMPOSE(RD::RasterizationState, r) {
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
		},
		COMPOSE(RD::MultisampleState, m) {
			m.sample_count = RD::TextureSamples_1;
			m.enable_sample_shading = false;
			m.sample_mask = {};
			m.enable_alpha_to_coverage = false;
			m.enable_alpha_to_one = false;
		},
		COMPOSE(RD::DepthStencilState, d) {
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
		},
		COMPOSE(RD::ColorBlendState, c) {
			c.enable_logic_op = true;
			c.logic_op = LogicOperation_Clear;
			c.blend_constant = Colors::clear;
			c.attachments.push_back({ true,
				RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
				RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
				true, true, true, true });
		});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static Mat4 object_matrix[] = // 
{
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	},
};

MEMBER_IMPL(EditorNode::singleton) {};

OBJECT_EMBED(EditorNode, t) {}

namespace ism
{
	template <size_t _Alignment, class ... _Types
	> class UniformConstantBuffer
	{
		static_assert(_Alignment % 2 == 0);

		static_assert(0 < sizeof...(_Types));

		template <size_t I
		> using _type = mpl::nth<I, mpl::type_list<_Types...>>;

		static constexpr size_t _align(size_t const value) noexcept
		{
			return IS_ALIGNED(value, _Alignment) ? value : SIZE_ROUND_UP(value, _Alignment);
		}

		template <size_t I = 0, size_t Result = 0
		> static constexpr size_t _calculate_size() noexcept
		{
			if constexpr (I < sizeof...(_Types))
			{
				return _calculate_size<I + 1, Result + _align(sizeof(_type<I>))>();
			}
			else
			{
				return Result;
			}
		}

		template <size_t Index, size_t I = 0, size_t Result = 0
		> static constexpr size_t _calculate_index() noexcept
		{
			static_assert(Index < sizeof...(_Types));

			if constexpr (I < Index)
			{
				return _calculate_index<Index, I + 1, Result + _align(sizeof(_type<I>))>();
			}
			else
			{
				return Result;
			}
		}

		static constexpr size_t _Size{ _calculate_size() };

		Array<byte, _Size> m_data{};

	public:
		using self_type			= typename UniformConstantBuffer<_Alignment, _Types...>;
		using storage_type		= typename decltype(m_data);
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		static constexpr byte null{ (byte)'\0' };

		UniformConstantBuffer() noexcept {}

		UniformConstantBuffer(self_type const & other) { copy(other); }
	
		UniformConstantBuffer(self_type && other) noexcept { swap(std::move(other)); }
	
		self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }
	
		self_type & operator=(self_type && other) noexcept { return swap(std::move(other)); }

	public:
		self_type & clear() noexcept {
			zeromem(m_data, _Size);
			return (*this);
		}

		self_type & copy(self_type const & other) {
			if (this != std::addressof(other)) { copymem(m_data, other.m_data, _Size); }
			return (*this);
		}

		self_type & swap(self_type & other) noexcept {
			if (this != std::addressof(other)) {
				byte temp[_Size];
				copymem(temp, m_data, _Size);
				copymem(m_data, other.m_data, _Size);
				copymem(other.m_data, temp, _Size);
			}
			return (*this);
		}

	public:
		NODISCARD bool empty() const noexcept { return false; }

		NODISCARD auto size() const noexcept -> size_t { return _Size; }

		NODISCARD auto capacity() const noexcept -> size_t { return _Size; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }

		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)data(); }

		NODISCARD auto begin() noexcept -> iterator { return data(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return data(); }

		NODISCARD auto end() noexcept -> iterator { return begin() + size(); }

		NODISCARD auto end() const noexcept -> const_iterator { return begin() + size(); }

		NODISCARD auto front() noexcept -> reference { return *(begin()); }

		NODISCARD auto front() const noexcept -> const_reference { return *(begin()); }

		NODISCARD auto back() noexcept -> reference { return *(end() - 1); }

		NODISCARD auto back() const noexcept -> const_reference { return *(end() - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> reference { return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { return m_data[i]; }

	public:
		void * get_to(size_t const index, void * dst, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= _Size);
			copymem(dst, begin() + index, size_in_bytes);
			return dst;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(size_t const index, T & value)
		{
			get_to(index, std::addressof(value), sizeof(T));
			return value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(T & value)
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t const index = 0)
		{
			T temp;
			return get_to<T>(index, temp);
		}

		template <size_t I
		> NODISCARD auto get() -> _type<I>
		{
			return get<_type<I>>(_calculate_index<I>());
		}

	public:
		void write_unchecked(size_t const index, void const * src, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= _Size);

			copymem(begin() + index, src, size_in_bytes);
		}

		self_type & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t const index, T const & value)
		{
			return write_unchecked(index, std::addressof(value), sizeof(T)), (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			return write_unchecked(0, std::addressof(value), sizeof(T)), (*this);
		}

		template <size_t I
		> self_type & set(mpl::nth<I, mpl::type_list<_Types...>> const & value)
		{
			return write(_calculate_index<I>(), value);
		}

	public:
		self_type & vprintf(size_t const index, cstring fmt, va_list args)
		{
			if (int32_t size_in_bytes{ std::vsnprintf(nullptr, 0, fmt, args) }; 0 < size_in_bytes)
			{
				size_in_bytes++; // account for null teminator

				ASSERT(index + size_in_bytes <= _Size);

				std::vsprintf((char *)begin() + index, fmt, args);
			}
			return (*this);
		}

		self_type & vprintf(cstring fmt, va_list args)
		{
			return vprintf(0, fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(0, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, String const & str)
		{
			if (!str.empty()) { write_unchecked(index, str.data(), str.size()); }
			return (*this);
		}

		self_type & print(String const & str)
		{
			if (!str.empty()) { write_unchecked(0, str.data(), str.size()); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(index, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str)
		{
			if (str) { write_unchecked(index, str, std::strlen(str)); }
			return (*this);
		}

		self_type & print(cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(0, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(cstring str)
		{
			if (str) { write_unchecked(0, str, std::strlen(str)); }
			return (*this);
		}
	};
}

EditorNode::EditorNode()
{
	ASSERT(!singleton);
	singleton = this;

	m_textures["earth_dm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_dm_2k.png");
	m_meshes["sphere32x24"].instance("../../../assets/meshes/sphere32x24.obj");
	m_shaders["3d"].instance("../../../assets/shaders/3d.json");

	RID const shader{ m_shaders["3d"]->get_rid() };
	_setup_pipeline(shader);

	uniform_buffers[SCENE] = RENDERING_DEVICE->uniform_buffer_create(UniformConstantBuffer<16, Mat4, Mat4>().size());
	uniform_sets[SCENE] = RENDERING_DEVICE->uniform_set_create({
		{ RD::UniformType_UniformBuffer, SCENE, { uniform_buffers[SCENE] } },
	}, shader);

	uniform_buffers[RENDER_PASS] = nullptr;
	uniform_sets[RENDER_PASS] = RENDERING_DEVICE->uniform_set_create({
	}, shader);

	uniform_buffers[TRANSFORMS] = RENDERING_DEVICE->uniform_buffer_create(sizeof(Mat4) * 1);
	uniform_sets[TRANSFORMS] = RENDERING_DEVICE->uniform_set_create({
		{ RD::UniformType_UniformBuffer, TRANSFORMS, { uniform_buffers[TRANSFORMS] } },
	}, shader);

	uniform_buffers[MATERIAL] = nullptr;
	uniform_sets[MATERIAL] = RENDERING_DEVICE->uniform_set_create({
		{ RD::UniformType_Texture, 0, { m_textures["earth_dm_2k"]->get_rid() } },
	}, shader);

	Vector<RID> fb_textures{
		RENDERING_DEVICE->texture_create({ RD::TextureType_2D, RD::DataFormat_R8G8B8_UNORM, 1280, 720 }),
		RENDERING_DEVICE->texture_create({ RD::TextureType_2D, RD::DataFormat_D24_UNORM_S8_UINT, 1280, 720 }) };
	framebuffer = RENDERING_DEVICE->framebuffer_create(fb_textures);
	m_editor_view.set_main_texture(fb_textures[0]);
}

EditorNode::~EditorNode()
{
	ASSERT(this == singleton);
	SCOPE_EXIT(&) { singleton = nullptr; };

	for (RID uniform_set : uniform_sets) { if (uniform_set) { RENDERING_DEVICE->uniform_set_destroy(uniform_set); } }
	for (RID uniform_buffer : uniform_buffers) { if (uniform_buffer) { RENDERING_DEVICE->uniform_buffer_destroy(uniform_buffer); } }
	if (framebuffer) { RENDERING_DEVICE->framebuffer_destroy(framebuffer); }
	if (backbuffer) { RENDERING_DEVICE->framebuffer_destroy(backbuffer); }
	if (pipeline) { RENDERING_DEVICE->pipeline_destroy(pipeline); }
}

void EditorNode::process(Duration const dt)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", (float_t)get_tree()->get_fps());
	get_tree()->get_root()->set_title(window_title);

	static EditorCamera * editor_camera{ m_editor_view.get_editor_camera() };
	static Vec2 view_size{ 1280, 720 }, view_size_prev{};
	if (m_editor_view.get_window()) { view_size = m_editor_view->InnerRect.GetSize(); }
	bool const viewport_resized{ view_size != view_size_prev };
	editor_camera->set_res(view_size);
	editor_camera->recalculate();
	if (viewport_resized) {
		view_size_prev = view_size;
		RENDERING_DEVICE->framebuffer_set_size(framebuffer, (int32_t)view_size[0], (int32_t)view_size[1]);
	}

	{
		static UniformConstantBuffer<16, Mat4, Mat4> scene_ubo_data;
		if (viewport_resized) { scene_ubo_data.set<0>(editor_camera->get_proj()); }
		scene_ubo_data.set<1>(editor_camera->get_view());
		RENDERING_DEVICE->uniform_buffer_update(uniform_buffers[SCENE], 0, scene_ubo_data.data(), scene_ubo_data.size());

		static UniformConstantBuffer<16, Mat4> transforms_ubo_data;
		transforms_ubo_data.set<0>(object_matrix[0]);
		RENDERING_DEVICE->uniform_buffer_update(uniform_buffers[TRANSFORMS], 0, transforms_ubo_data.data(), transforms_ubo_data.size());

		static Color clear_color{ Colors::magenta };
		clear_color = rotate_hue(clear_color, (float_t)dt * 10.f);
		RID dl{ RENDERING_DEVICE->drawlist_begin(framebuffer, clear_color) };
		RENDERING_DEVICE->drawlist_bind_pipeline(dl, pipeline);
		RENDERING_DEVICE->drawlist_bind_uniform_set(dl, uniform_sets[SCENE], SCENE);
		RENDERING_DEVICE->drawlist_bind_uniform_set(dl, uniform_sets[RENDER_PASS], RENDER_PASS);
		RENDERING_DEVICE->drawlist_bind_uniform_set(dl, uniform_sets[TRANSFORMS], TRANSFORMS);
		RENDERING_DEVICE->drawlist_bind_uniform_set(dl, uniform_sets[MATERIAL], MATERIAL);
		
		static Mesh * mesh{ *m_meshes["sphere32x24"] };
		mesh->for_each([&](RID vertex_array, RID index_array, Vector<Ref<Texture>> & textures) {
			RENDERING_DEVICE->drawlist_bind_vertex_array(dl, vertex_array);
			RENDERING_DEVICE->drawlist_bind_index_array(dl, index_array);
			RENDERING_DEVICE->drawlist_draw(dl, (bool)index_array, 1, 0);
		});
		
		RENDERING_DEVICE->drawlist_end();
	}

	_draw_dockspace();
	if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }
	m_hierarchy.process(dt);
	m_editor_log.process(dt);
	m_editor_view.process(dt);

	Node::process(dt);
}

void EditorNode::_draw_dockspace()
{
	static bool opt_fullscreen{ true };
	static bool opt_padding{ false };
	static ImGuiDockNodeFlags dockspace_flags{ ImGuiDockNodeFlags_None };
	
	ImGuiWindowFlags window_flags{ ImGuiWindowFlags_NoDocking };
	if (m_show_main_menu_bar) { window_flags |= ImGuiWindowFlags_MenuBar; }
	
	if (opt_fullscreen) {
		ImGuiViewport * viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else {
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) { window_flags |= ImGuiWindowFlags_NoBackground; }
	
	if (!opt_padding) { ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f }); }
	
	bool const dockspace_open{ ImGui::Begin("Editor", nullptr, window_flags) };
	
	if (!opt_padding) { ImGui::PopStyleVar(); }
	
	if (opt_fullscreen) { ImGui::PopStyleVar(2); }
	
	ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockSpace") };

	if (dockspace_open && !ImGui::DockBuilderGetNode(dockspace_id)) {
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id);
		_build_dockspace();
		ImGui::DockBuilderFinish(dockspace_id);
	}
	
	ImGui::DockSpace(dockspace_id, { 0.f, 0.f }, dockspace_flags);

	if (m_show_main_menu_bar && ImGui::BeginMenuBar()) {
		_draw_menu_bar();
		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void EditorNode::_build_dockspace()
{
	ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockSpace") };
	ImGuiID right{ dockspace_id };
	ImGuiID left_up{ ImGui::DockBuilderSplitNode(right, ImGuiDir_Left, 0.2f, nullptr, &right) };
	ImGuiID left_down{ ImGui::DockBuilderSplitNode(left_up, ImGuiDir_Down, 0.5f, nullptr, &left_up) };
	ImGui::DockBuilderDockWindow(m_editor_view.get_name(), right);
	ImGui::DockBuilderDockWindow(m_hierarchy.get_name(), left_up);
	ImGui::DockBuilderDockWindow(m_editor_log.get_name(), left_down);
}

void EditorNode::_draw_menu_bar()
{
	if (ImGui::BeginMenu("File")) {
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Alt+F4")) { get_tree()->get_root()->set_should_close(true); }
		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Dear ImGui Demo")) { m_show_imgui_demo = !m_show_imgui_demo; }
		if (ImGui::MenuItem(m_hierarchy.get_name(), "", m_hierarchy.is_open())) { m_hierarchy.toggle_open(); }
		if (ImGui::MenuItem(m_editor_log.get_name(), "", m_editor_log.is_open())) { m_editor_log.toggle_open(); }
		if (ImGui::MenuItem(m_editor_view.get_name(), "", m_editor_view.is_open())) { m_editor_view.toggle_open(); }
		ImGui::EndMenu();
	}
}
