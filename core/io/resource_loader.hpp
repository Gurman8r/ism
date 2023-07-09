#ifndef _ISM_RESOURCE_LOADER_HPP_
#define _ISM_RESOURCE_LOADER_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ResourceFormatLoader : public Object
	{
		OBJECT_CLASS(ResourceFormatLoader, Object);
		friend class ResourceLoader;
	public:
		virtual ~ResourceFormatLoader() noexcept override = default;
		NODISCARD virtual RES load(String const & path, Error_ * r_error = nullptr) = 0;
		NODISCARD virtual void get_recognized_extensions(Vector<String> * out) const = 0;
		NODISCARD virtual bool recognize_path(String const & path, String const & hint = {}) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ResourceLoader final : public Object
	{
		OBJECT_CLASS(ResourceLoader, Object);

		static ResourceLoader * __singleton;

		Vector<Ref<ResourceFormatLoader>> m_loaders{};

	public:
		ResourceLoader() noexcept { SINGLETON_CTOR(); }
		~ResourceLoader() noexcept { SINGLETON_DTOR(); }
		SINGLETON_GETTER(ResourceLoader);

	public:
		NODISCARD RES load(String const & path, Error_ * r_error = nullptr);

		template <class T
		> NODISCARD Ref<T> load(String const & path, Error_ * r_error = nullptr) noexcept { return load(path, r_error); }

		bool add_resource_format_loader(Ref<ResourceFormatLoader> format);
		bool remove_resource_format_loader(Ref<ResourceFormatLoader> format);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ResourceLoader, get_loaders);

	NODISCARD inline RES load_resource(String const & path, Error_ * r_error = nullptr) noexcept { return get_loaders()->load(path, r_error); }

	template <class T
	> NODISCARD Ref<T> load_resource(String const & path, Error_ * r_error = nullptr) noexcept { return get_loaders()->load<T>(path, r_error); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
