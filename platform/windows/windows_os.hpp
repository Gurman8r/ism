#ifndef _ISM_WINDOWS_OS_HPP_
#define _ISM_WINDOWS_OS_HPP_

#include <platform/windows/windows.hpp>

#include <core/os/os.hpp>

static_assert(ISM_OS_WINDOWS);

namespace ism
{
	class ISM_API Windows_OS : public OS
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MainLoop *	m_main_loop{};
		HINSTANCE	m_instance{};
		HWND		m_main_window{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
	public:
		virtual ~Windows_OS() override;

		explicit Windows_OS(HINSTANCE hInstance);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Error open_dynamic_library(String const & path, void *& instance) override;
		
		virtual Error close_dynamic_library(void * instance) override;
		
		virtual Error get_dynamic_library_symbol_handle(void * instance, String const & name, void *& symbol, bool is_optional = false) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual bool has_environment(String const & key) const override;
		
		NODISCARD virtual String get_environment(String const & key) const override;
		
		virtual bool set_environment(String const & key, String const & value) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		virtual String get_stdin_string(bool block = true) override;
		
		virtual Error set_cwd(String const & path) override;
		
		NODISCARD virtual String get_name() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void set_main_window(HWND value) { m_main_window = value; }
		
		HINSTANCE get_instance() { return m_instance; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		virtual void initialize() override;

		virtual void finalize() override;

		virtual void finalize_core() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual MainLoop * get_main_loop() const override;

		virtual void set_main_loop(MainLoop * value) override;

		virtual void delete_main_loop() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_WINDOWS_OS_HPP_
