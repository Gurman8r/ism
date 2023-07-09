#ifndef _ISM_RESOURCE_SAVER_HPP_
#define _ISM_RESOURCE_SAVER_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ResourceFormatSaver : public Object
	{
		OBJECT_CLASS(ResourceFormatSaver, Object);
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

	class ISM_API ResourceSaver final : public Object
	{
		OBJECT_CLASS(ResourceSaver, Object);

		static ResourceSaver * __singleton;

		Vector<Ref<ResourceFormatSaver>> m_savers{};

	public:
		ResourceSaver() noexcept { SINGLETON_CTOR(); }
		~ResourceSaver() noexcept { SINGLETON_DTOR(); }
		SINGLETON_GETTER(ResourceSaver);

	public:
		Error_ save(RES const & value, String const & path, i32 flags = 0);

		template <class T
		> Error_ save(Ref<T> const & value, String const & path, i32 flags = 0) noexcept { return save(value, path, flags); }

		bool add_resource_format_saver(Ref<ResourceFormatSaver> format);
		bool remove_resource_format_saver(Ref<ResourceFormatSaver> format);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ResourceSaver, get_savers);

	inline Error_ save_resource(RES const & value, String const & path, i32 flags = 0) noexcept { return get_savers()->save(value, path, flags); }

	template <class T
	> Error_ save_resource(Ref<T> const & value, String const & path, i32 flags = 0) noexcept { return get_savers()->save<T>(value, path, flags); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
