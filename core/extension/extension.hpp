#ifndef _ISM_EXTENSION_HPP_
#define _ISM_EXTENSION_HPP_

#include <core/extension/extension_interface.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/config_file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Extension : public Resource
	{
		OBJECT_CLASS(Extension, Resource);

		String m_library_path{};
		void * m_library{};
		i32 m_level_initialized{ -1 };
		ExtensionInitialization m_initialization{};
		ConfigFile m_ini{};

	public:
		Extension() noexcept {}
		virtual ~Extension() noexcept override;

		Error_ open_library(String const & path, String const & entry_symbol);
		void close_library();

		NODISCARD bool is_library_open() const;
		NODISCARD ExtensionInitializationLevel_ get_minimum_level() const;
		
		void initialize_library(ExtensionInitializationLevel_ level);
		void finalize_library(ExtensionInitializationLevel_ level);

		NODISCARD auto get_ini() noexcept -> ConfigFile & { return m_ini; }
		NODISCARD auto get_ini() const noexcept -> ConfigFile const & { return m_ini; }
		void set_ini(ConfigFile const & value) { m_ini = value; }

	public:
		static void initialize_interface();
		static String get_extension_list_config_file();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ExtensionFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(ExtensionFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * r_out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EXTENSION_HPP_
