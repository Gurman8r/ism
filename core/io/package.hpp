#ifndef _ISM_PACKAGE_HPP_
#define _ISM_PACKAGE_HPP_

#include <core/io/dir.hpp>
#include <core/io/file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum PackageDirFlags_ { PackageDirFlags_Encrypted = 1 << 0 };

	enum PackageFileFlags_ { PackageFileFlags_Encrypted = 1 << 0 };

	class PackageSource;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// packed data
	class ISM_API PackageManager
	{
	public:
		using PathID = size_t;

		struct PackFile {
			String pack{};
			u64 offset{}; //if offset is ZERO, the file was ERASED
			u64 size{};
			PathID id{};
			PackageSource * src{};
			bool encrypted{};
		};

		struct PackDir {
			PackDir * parent{};
			String name{};
			HashMap<String, PackDir *> subdirs{};
			HashSet<String> files{};
			~PackDir() { for (auto & [k, v] : subdirs) { memdelete(v); } }
		};

	private:
		static PackageManager * __singleton;
		bool m_enabled{ true };
		HashMap<String, PackFile> m_files{};
		Vector<PackageSource *> m_sources{};
		PackDir * m_root{};

	public:
		PackageManager();
		~PackageManager();
		FORCE_INLINE static PackageManager * get_singleton() noexcept { return __singleton; }

		NODISCARD bool is_enabled() const { return m_enabled; }
		void set_enabled(bool enabled) { m_enabled = enabled; }

		Error_ add_package(String const & path, bool replace_files, u64 offset);
		void add_package_source(PackageSource * source);
		void add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackageSource * src, bool replace_files, bool encrypted = false);

		NODISCARD Ref<File> try_open_path(String const & path);
		NODISCARD bool has_path(String const & path);

		NODISCARD Ref<Dir> try_open_dir(String const & path);
		NODISCARD bool has_dir(String const & path);
	};

	SINGLETON_WRAPPER(PackageManager, get_packages);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack source
	class ISM_API PackageSource {
	public:
		virtual ~PackageSource() noexcept = default;
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) = 0;
		virtual Ref<File> get_file(String const & path, PackageManager::PackFile * file) = 0;
	};

	// pack source PCK
	class ISM_API PackageSourcePCK : public PackageSource {
	public:
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		virtual Ref<File> get_file(String const & path, PackageManager::PackFile * file) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file access pack
	class ISM_API PackageFile : public Object
	{
		DEFINE_CLASS(PackageFile, Object);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dir access pack
	class ISM_API PackageDir : public Object
	{
		DEFINE_CLASS(PackageDir, Object);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PACKAGE_FILE_HPP_
