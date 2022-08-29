#include <core/extension/native_extension.hpp>
#include <core/extension/native_extension_manager.hpp>
#include <core/os/os.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(NativeExtension, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NativeExtension::NativeExtension()
	{
	}

	NativeExtension::~NativeExtension()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ NativeExtension::open_library(Path const & path, String const & entry_symbol)
	{
		void * instance;
		
		Error_ err{ OS::get_singleton()->open_dynamic_library(path, instance) };

		return err;
	}

	void NativeExtension::close_library()
	{
	}

	bool NativeExtension::is_library_open() const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NativeExtension::InitializationLevel_ NativeExtension::get_minimum_library_initialization_level() const
	{
		return InitializationLevel_();
	}

	void NativeExtension::initialize_library(InitializationLevel_ level)
	{
	}

	void NativeExtension::deinitialize_library(InitializationLevel_ level)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}