#ifndef _SYSTEM_WINDOWS_HPP_
#define _SYSTEM_WINDOWS_HPP_

#include <core/os/os.hpp>

#include <platform/windows/windows.hpp>

static_assert(SYSTEM_WINDOWS);

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API OS_Windows : public OS
	{
		Ref<MainLoop> m_main_loop{};
		HINSTANCE m_instance{};
		HWND m_main_window{};

	public:
		explicit OS_Windows(HINSTANCE hInstance);

		virtual ~OS_Windows() override;

		void set_main_window(HWND value) { m_main_window = value; }
		
		HINSTANCE get_instance() { return m_instance; }

	protected:
		virtual void initialize() override;

		virtual void finalize() override;

		virtual void finalize_core() override;

	public:
		NODISCARD virtual bool has_environment(String const & key) const override;

		NODISCARD virtual String get_environment(String const & key) const override;

		virtual bool set_environment(String const & key, String const & value) const override;

	public:
		virtual String get_cwd() const override;

		virtual Error set_cwd(String const & path) override;

		NODISCARD virtual String get_name() const override;

		virtual String get_stdin_string(bool block = true) override;

	public:
		virtual Ref<MainLoop> get_main_loop() const override;

		virtual void set_main_loop(Ref<MainLoop> value) override;

		virtual void delete_main_loop() override;

	public:
		virtual Error open_dynamic_library(String const & path, void *& instance) override;

		virtual Error close_dynamic_library(void * instance) override;

		virtual Error get_dynamic_library_symbol_handle(void * instance, String const & name, void *& symbol, bool is_optional = false) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SYSTEM_WINDOWS_HPP_
