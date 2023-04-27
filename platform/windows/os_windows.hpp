#ifndef _ISM_OS_WINDOWS_HPP_
#define _ISM_OS_WINDOWS_HPP_

#include <core/os/os.hpp>

#include <platform/windows/windows.hpp>

static_assert(SYSTEM_WINDOWS);

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// windows operating system
	class ISM_API OS_Windows : public OS
	{
		Ref<MainLoop> m_main_loop{};
		HINSTANCE m_hinstance{};
		HWND m_main_window{};

	public:
		explicit OS_Windows(HINSTANCE hInstance);
		virtual ~OS_Windows() override;

		HINSTANCE get_instance() { return m_hinstance; }
		void set_main_window(HWND value) { m_main_window = value; }

		virtual void initialize() override;
		virtual void initialize_joysticks() override;
		virtual void finalize() override;
		virtual void finalize_core() override;

		virtual void run() override;

		virtual Vector<String> get_video_adapter_driver_info() const override;

		virtual String get_stdin_string(bool block = true) override;

		virtual Error_ open_dynamic_library(Path const & path, void *& instance) override;
		virtual Error_ close_dynamic_library(void * instance) override;
		virtual Error_ get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional = false) override;

		virtual Error_ execute(Path const & path, Vector<String> const & args, String * pipe = nullptr, i32 * exitcode = nullptr, bool read_stderr = false, Mutex * pipe_mutex = nullptr) override;
		virtual Error_ create_process(Path const & path, Vector<String> const & args, ProcessID * child_id = nullptr) override;
		virtual Error_ kill(ProcessID const & pid) override;
		virtual i32 get_pid() const override;

		virtual Path get_cwd() const override;
		virtual Error_ set_cwd(Path const & path) override;
		virtual Error_ shell_open(Path const & path) override;

		virtual Path get_env(String const & key) const override;
		virtual bool has_env(String const & key) const override;
		virtual bool set_env(String const & key, Path const & value) const override;

		virtual String get_name() const override;
		virtual String get_model_name() const;

		virtual Ref<MainLoop> get_main_loop() const override;
		virtual void set_main_loop(Ref<MainLoop> value) override;
		virtual void delete_main_loop() override;

		virtual Date get_date(bool local = false) const override;
		virtual Time get_time(bool local = false) const override;
		virtual TimeZoneInfo get_time_zone() const override;

		virtual void debug_break() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OS_WINDOWS_HPP_
