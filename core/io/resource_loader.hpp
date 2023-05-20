#ifndef _ISM_RESOURCE_LOADER_HPP_
#define _ISM_RESOURCE_LOADER_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API ResourceFormatLoader : public Object
	{
		DEFINE_CLASS(ResourceFormatLoader, Object);
		friend class ResourceLoader;
	public:
		virtual ~ResourceFormatLoader() noexcept override = default;
		NODISCARD virtual RES load(String const & path, Error_ * r_error = nullptr) = 0;
		NODISCARD virtual void get_recognized_extensions(Vector<String> * out) const = 0;
		NODISCARD virtual bool recognize_path(String const & path, String const & hint = {}) const;
	};
}

namespace Ism
{
	class ISM_API ResourceLoader
	{
		static ResourceLoader * __singleton;

		Vector<Ref<ResourceFormatLoader>> m_loaders{};

	public:
		ResourceLoader();
		~ResourceLoader();
		FORCE_INLINE static ResourceLoader * get_singleton() noexcept { return __singleton; }

	protected:
		RES _load(String const & path, Error_ * r_error = nullptr);

	public:
		RES load(String const & path, Error_ * r_error = nullptr);
		bool add(Ref<ResourceFormatLoader> format);
		bool remove(Ref<ResourceFormatLoader> format);
	};

	SINGLETON_WRAPPER(ResourceLoader, get_resource_loader);
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
