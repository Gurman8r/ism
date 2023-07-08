#ifndef _ISM_RESOURCE_SAVER_HPP_
#define _ISM_RESOURCE_SAVER_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ResourceFormatSaver : public Object
	{
		DEFINE_CLASS(ResourceFormatSaver, Object);
		friend class ResourceSaver;
	public:
		virtual ~ResourceFormatSaver() noexcept override = default;
		NODISCARD virtual Error_ save(RES const & value, String const & path, i32 flags = 0) = 0;
		NODISCARD virtual void get_recognized_extensions(Vector<String> * out) const = 0;
		NODISCARD virtual bool recognize_path(String const & path, String const & hint = {}) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum SaverFlags_ {
		SaverFlags_None,
		SaverFlags_RelativePaths			= 1 << 0,
		SaverFlags_BundleResources			= 1 << 1,
		SaverFlags_ChangePath				= 1 << 2,
		SaverFlags_OmitEditorProperties		= 1 << 3,
		SaverFlags_SaveBigEndian			= 1 << 4,
		SaverFlags_Compress					= 1 << 5,
		SaverFlags_ReplaceSubresourcePaths	= 1 << 6,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ResourceSaver
	{
		static ResourceSaver * __singleton;

		Vector<Ref<ResourceFormatSaver>> m_savers{};

	public:
		ResourceSaver() noexcept { SINGLETON_CTOR(__singleton, this); }
		~ResourceSaver() noexcept { SINGLETON_DTOR(__singleton, this); }
		SINGLETON_GETTER(ResourceSaver, __singleton);

	public:
		static Error_ save(RES const & value, String const & path, i32 flags = 0);

		static bool add_resource_format_saver(Ref<ResourceFormatSaver> format);
		static bool remove_resource_format_saver(Ref<ResourceFormatSaver> format);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
