#ifndef _ISM_PACK_HPP_
#define _ISM_PACK_HPP_

#include <core/io/dir.hpp>
#include <core/io/file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum PackageDirFlags_ { PackageDirFlags_Encrypted = 1 << 0 };

	enum PackageFileFlags_ { PackageFileFlags_Encrypted = 1 << 0 };

	class PackSource;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// package manager
	class ISM_API PackedData
	{
	public:
		using PathID = size_t;

		struct PackedFile {
			String pack{};
			u64 offset{}; //if offset is ZERO, the file was ERASED
			u64 size{};
			PathID id{};
			PackSource * src{};
			bool encrypted{};
		};

		struct PackedDir {
			PackedDir * parent{};
			String name{};
			HashMap<String, PackedDir *> subdirs{};
			HashSet<String> files{};
			~PackedDir() { for (auto & [k, v] : subdirs) { memdelete(v); } }
		};

	private:
		static PackedData * __singleton;
		bool m_enabled{ true };
		HashMap<String, PackedFile> m_files{};
		Vector<PackSource *> m_sources{};
		PackedDir * m_root{};

	public:
		PackedData();
		~PackedData();
		FORCE_INLINE static PackedData * get_singleton() noexcept { return __singleton; }

		NODISCARD bool is_enabled() const { return m_enabled; }
		void set_enabled(bool enabled) { m_enabled = enabled; }

		Error_ add_package(String const & path, bool replace_files, u64 offset);
		void add_package_source(PackSource * source);
		void add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted = false);

		NODISCARD Ref<File> try_open_path(String const & path);
		NODISCARD bool has_path(String const & path);

		NODISCARD Ref<Dir> try_open_dir(String const & path);
		NODISCARD bool has_dir(String const & path);
	};

	SINGLETON_WRAPPER(PackedData, get_packed_data);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack source
	class ISM_API PackSource {
	public:
		virtual ~PackSource() noexcept = default;
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) = 0;
		virtual Ref<File> get_file(String const & path, PackedData::PackedFile * file) = 0;
	};

	// pack source PAK
	class ISM_API PackSourcePAK : public PackSource {
	public:
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		virtual Ref<File> get_file(String const & path, PackedData::PackedFile * file) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack file
	class ISM_API PackFile : public Object
	{
		DEFINE_CLASS(PackFile, Object);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack directory
	class ISM_API PackDir : public Object
	{
		DEFINE_CLASS(PackDir, Object);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PACKAGE_FILE_HPP_
