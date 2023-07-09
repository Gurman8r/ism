#if TOOLS_ENABLED

#include <editor/editor_terminal.hpp>
#include <core/object/script.hpp>
#include <core/os/os.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EditorTerminal, t) {}

	EditorTerminal::EditorTerminal() : EditorPanel{ "Terminal##Editor" }
	{
		get_os()->get_logger()->add_logger(m_logger = memnew(EditorTerminalLogger(this)));

		m_script = get_scr()->get_language("lua")->new_scipt();

		m_commands.push_back("HELP");
		m_commands.push_back("HISTORY");
		m_commands.push_back("CLEAR");
		m_commands.push_back("CLASSIFY");
		this->printf("Welcome to Dear ImGui!");
	}

	EditorTerminal::~EditorTerminal()
	{
		if (m_script) { memdelete(m_script); m_script = nullptr; }

		get_os()->get_logger()->remove_logger(m_logger);
		if (m_logger) { memdelete(m_logger); m_logger = nullptr; }
	}

	void EditorTerminal::process(Duration const & delta_time)
	{
		if (!is_open()) { return; }
		ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 320.f, 180.f }, ImGuiCond_FirstUseEver);
		bool open; EDITOR_PANEL_SCOPE(open);
		if (!open) { return; }

		// context menu
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("close")) {
				set_open(false);
			}
			ImGui::EndPopup();
		}
		ImGui::Separator();

		// options menu
		bool copy_to_clipboard{};
		if (ImGui::BeginPopup("options")) {
			copy_to_clipboard |= ImGui::Button("copy to clipboard");
			ImGui::Checkbox("auto-scroll", &m_autoscroll);
			ImGui::EndPopup();
		}
		// options button
		if (ImGui::Button("options")) {
			ImGui::OpenPopup("options");
		}
		// filter
		ImGui::SameLine();
		m_filter.Draw("filter", 180);
		ImGui::Separator();

		// reserve enough left-over height for 1 separator + 1 input text
		f32 const footer_height_to_reserve{ ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() };
		ImGui::BeginChild("ScrollingRegion", Vec2{ 0, -footer_height_to_reserve }, false, ImGuiWindowFlags_HorizontalScrollbar);
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("clear")) {
				clear();
			}
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVarType_ItemSpacing, Vec2{ 4, 1 }); // tighten spacing
		if (copy_to_clipboard) {
			ImGui::LogToClipboard();
		}

		for (size_t i{}; i < m_items.size(); ++i) {
			cstring item{ m_items[i].c_str() };
			if (!m_filter.PassFilter(item)) { continue; }
			ImGui::TextUnformatted(item);
		}

		if (copy_to_clipboard) {
			ImGui::LogFinish();
		}
		
		if (m_scroll_to_bottom || (m_autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
			ImGui::SetScrollHereY(1.0f);
		}
		m_scroll_to_bottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// command-line
		bool reclaim_focus{};
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("input", m_input, m_input.size(), input_text_flags, [](auto data) { return ((EditorTerminal *)data->UserData)->text_edit_callback(data); }, this)) {
			if (auto const s{ String(m_input).trim() }; !s.empty()) { execute(s); }
			strcpy(m_input, "");
			reclaim_focus = true;
		}
		// autofocus on window apparition
		ImGui::SetItemDefaultFocus();
		// autofocus previous widget
		if (reclaim_focus){ ImGui::SetKeyboardFocusHere(-1); }
	}

	void EditorTerminal::clear()
	{
		m_items.clear();
	}

	void EditorTerminal::printv(cstring fmt, va_list args)
	{
		if (String s{ String::format<1024>(fmt, args) }; !s.empty()) {
			m_items.emplace_back(std::move(s));
		}
	}

	void EditorTerminal::printf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		this->printv(fmt, args);
		va_end(args);
	}

	void EditorTerminal::execute(String const & command_line)
	{
		this->printf("# %.*s\n", command_line.size(), command_line.data());
		m_history_pos = -1;
		for (ssize_t i{ (ssize_t)m_history.size() - 1 }; i >= 0; i--) {
			if (m_history[(size_t)i] == command_line) {
				m_history.erase(m_history.begin() + i);
				break;
			}
		}
		m_history.push_back(command_line);

		m_script->set_source_code(command_line);
		m_script->reload();

		m_scroll_to_bottom = true;
	}

	i32 EditorTerminal::text_edit_callback(ImGuiInputTextCallbackData * data)
	{
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion: {

			// TEXT COMPLETION

			// locate beginning of current word
			cstring word_end{ data->Buf + data->CursorPos }, word_start{ word_end };
			while (word_start > data->Buf) {
				char const c{ word_start[-1] };
				if (c == ' ' || c == '\t' || c == ',' || c == ';') { break; }
				word_start--;
			}

			// gather candidates
			Vector<cstring> candidates{};
			for (size_t i{}; i < m_commands.size(); ++i) {
				if (auto const cmd{ m_commands[i].c_str() }
				; std::strncmp(cmd, word_start, (size_t)(word_end - word_start)) == 0) {
					candidates.push_back(cmd);
				}
			}

			// show candidates
			if (candidates.size() == 1) {
				data->DeleteChars((i32)(word_start - data->Buf), (i32)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else if (candidates.size() > 1)
			{
				size_t match_len{ (size_t)(word_end - word_start) };
				for (;;)
				{
					size_t c{};
					bool all_candidates_matches{ true };
					for (size_t i{}; i < candidates.size() && all_candidates_matches; ++i) {
						if (i == 0){
							c = toupper(candidates[i][match_len]);
						}
						else if (c == 0 || c != toupper(candidates[i][match_len])) {
							all_candidates_matches = false;
						}
					}
					if (!all_candidates_matches) {
						break;
					}
					++match_len;
				}
				if (match_len > 0) {
					data->DeleteChars((i32)(word_start - data->Buf), (i32)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}
				for (size_t i{}; i < candidates.size(); ++i) {
					this->printf("- %s\n", candidates[i]);
				}
			}

		} break;
		case ImGuiInputTextFlags_CallbackHistory: {

			// HISTORY

			i32 const prev_history_pos{ m_history_pos };
			if (data->EventKey == ImGuiKey_UpArrow) {
				if (m_history_pos == -1) {
					m_history_pos = (i32)m_history.size() - 1;
				}
				else if (m_history_pos > 0) {
					m_history_pos--;
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow) {
				if (m_history_pos != -1) {
					if (++m_history_pos >= (i32)m_history.size()) {
						m_history_pos = -1;
					}
				}
			}

			// a better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != m_history_pos) {
				cstring history_str{ m_history_pos >= 0 ? m_history[m_history_pos].c_str() : "" };
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}

		} break;
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorTerminalLogger::logv(cstring fmt, va_list args, bool is_error)
	{
		VALIDATE(m_terminal)->printv(fmt, args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // TOOLS_ENABLED