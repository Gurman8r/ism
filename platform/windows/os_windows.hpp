#ifndef _ISM_OS_WINDOWS_HPP_
#define _ISM_OS_WINDOWS_HPP_

// target Windows 7 or later
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

// features
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define STRICT

#include <windows.h>

#include <core/os/os.hpp>

static_assert(ISM_OS_WINDOWS);

namespace ism
{
	class ISM_API OS_Windows : public OS
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MainLoop *	m_main_loop{};
		HINSTANCE	m_instance{};
		HWND		m_main_window{};

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

	public:
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
		
		OS_Windows(HINSTANCE instance);
		
		virtual ~OS_Windows();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_OS_WINDOWS_HPP_
