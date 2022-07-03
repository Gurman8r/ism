#ifndef _ISM_OS_WINDOWS_HPP_
#define _ISM_OS_WINDOWS_HPP_

#include <core/os/os.hpp>

#include <platform/windows/windows.hpp>

static_assert(OS_WINDOWS);

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

	protected:
		virtual void initialize() override;
		virtual void finalize() override;
		virtual void finalize_core() override;
		virtual void set_main_loop(Ref<MainLoop> value) override;
		virtual void delete_main_loop() override;

	public:
		virtual String get_name() const override;
		virtual String get_stdin_string(bool block = true) override;
		virtual String get_env(String const & key) const override;
		virtual bool has_env(String const & key) const override;
		virtual bool set_env(String const & key, String const & value) const override;
		virtual Path get_cwd() const override;
		virtual Error_ set_cwd(Path const & path) override;
		virtual Ref<MainLoop> get_main_loop() const override;

	public:
		virtual Error_ open_dynamic_library(Path const & path, void *& instance) override;
		virtual Error_ close_dynamic_library(void * instance) override;
		virtual Error_ get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional = false) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OS_WINDOWS_HPP_
