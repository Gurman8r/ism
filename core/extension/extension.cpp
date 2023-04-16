#include <core/extension/extension.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ExtensionInterface extension_interface{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Extension, t) {}

	Extension::Extension()
	{
	}

	Extension::~Extension()
	{
		if (m_library) {
			close_library();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ Extension::open_library(Path const & path, String const & entry_symbol)
	{
		if (Error_ const err{ OS::get_singleton()->open_dynamic_library(path, m_library) }) {
			return err;
		}
		
		void * entry_func{};
		if (Error_ const err{ OS::get_singleton()->get_dynamic_library_symbol(m_library, entry_symbol, entry_func, false) }) {
			OS::get_singleton()->close_dynamic_library(m_library);
			return err;
		}

		ExtensionInitializationFunction initialization_function{ (ExtensionInitializationFunction)entry_func };
		if (initialization_function(&extension_interface, this, &m_initialization)) {
			m_level_initialized = -1;
			return Error_None;
		}
		else {
			return Error_Unknown;
		}
	}

	void Extension::close_library()
	{
		if (m_library) {
			OS::get_singleton()->close_dynamic_library(m_library);
			m_library = nullptr;
		}
	}

	bool Extension::is_library_open() const
	{
		return m_library;
	}

	ExtensionInitializationLevel_ Extension::get_minimum_library_initialization_level() const
	{
		return m_initialization.minimum_level;
	}

	void Extension::initialize_library(ExtensionInitializationLevel_ level)
	{
		ASSERT(m_library);
		ASSERT(level <= m_level_initialized);
		m_level_initialized = (i32)level;
		ASSERT(m_initialization.initialize);
		m_initialization.initialize(m_initialization.userdata, level);
	}

	void Extension::finalize_library(ExtensionInitializationLevel_ level)
	{
		ASSERT(m_library);
		ASSERT(level > m_level_initialized);
		m_level_initialized = (i32)level - 1;
		m_initialization.finalize(m_initialization.userdata, level);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Extension::initialize_extensions()
	{
	}

	Path Extension::get_extension_list_config_file()
	{
		return ProjectSettings::get_singleton()->get_project_data_path();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}