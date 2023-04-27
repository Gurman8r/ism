#ifndef _ISM_RESOURCE_LOADER_HPP_
#define _ISM_RESOURCE_LOADER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ResourceFormatLoader : public Object
	{
		DEFINE_CLASS(ResourceFormatLoader, Object);
		friend class ResourceLoader;
	public:
		virtual ~ResourceFormatLoader() noexcept override = default;
		NODISCARD virtual RES load(Path const & path, Error_ * error = nullptr) = 0;
		NODISCARD virtual void get_recognized_extensions(Vector<Path> * out) const = 0;
		NODISCARD virtual TYPE get_resource_type(Path const & path) const = 0;
		NODISCARD virtual bool recognize_path(Path const & path, String const & hint = {}) const;
	};
}

namespace ism
{
	class ISM_API ResourceLoader
	{
		static ResourceLoader * __singleton;

		Vector<Ref<ResourceFormatLoader>> m_loaders{};

	public:
		ResourceLoader();
		~ResourceLoader();
		FORCE_INLINE static ResourceLoader * get_singleton() noexcept { return __singleton; }
#define RESOURCE_LOADER (ism::ResourceLoader::get_singleton())

	protected:
		RES _load(Path const & path, Error_ * error = nullptr);

	public:
		RES load(Path const & path, Error_ * error = nullptr);
		bool add(Ref<ResourceFormatLoader> format);
		bool remove(Ref<ResourceFormatLoader> format);
	};
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
