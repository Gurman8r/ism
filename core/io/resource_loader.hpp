#ifndef _ISM_RESOURCE_LOADER_HPP_
#define _ISM_RESOURCE_LOADER_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ResourceLoader
	{
		static ResourceLoader * __singleton;

		Vector<Ref<ResourceFormatLoader>> m_loaders{};

	public:
		ResourceLoader() noexcept { SINGLETON_CTOR(__singleton, this); }
		~ResourceLoader() noexcept { SINGLETON_DTOR(__singleton, this); }
		SINGLETON_GETTER(ResourceLoader, __singleton);

	protected:
		static RES _load(String const & path, Error_ * r_error = nullptr);

	public:
		static RES load(String const & path, Error_ * r_error = nullptr);

		template <class T
		> static Ref<T> load(String const & path, Error_ * r_error = nullptr) noexcept { return load(path, r_error); }

		static bool add_resource_format_loader(Ref<ResourceFormatLoader> format);
		static bool remove_resource_format_loader(Ref<ResourceFormatLoader> format);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
