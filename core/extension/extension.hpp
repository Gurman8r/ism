#ifndef _ISM_EXTENSION_HPP_
#define _ISM_EXTENSION_HPP_

#include <core/extension/extension_interface.hpp>

#include <core/io/resource_loader.hpp>

namespace ism
{
	class ISM_API Extension : public Resource
	{
		DEFINE_CLASS(Extension, Resource);

		Path m_library_path{};
		void * m_library{};

		ExtensionInitialization m_initialization{};

		i32 m_level_initialized{};

	public:
		Extension();
		virtual ~Extension() override;

		Error_ open_library(Path const & path, String const & entry_symbol);
		void close_library();

		NODISCARD bool is_library_open() const;
		NODISCARD ExtensionInitializationLevel_ get_minimum_library_initialization_level() const;

		void initialize_library(ExtensionInitializationLevel_ level);
		void finalize_library(ExtensionInitializationLevel_ level);

		static void initialize_extensions();
		static Path get_extension_list_config_file();
	};
}

#endif // !_ISM_EXTENSION_HPP_
