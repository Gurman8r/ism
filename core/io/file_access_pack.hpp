#ifndef _ISM_FILE_ACCESS_PACK_HPP_
#define _ISM_FILE_ACCESS_PACK_HPP_

#include <core/io/dir_access.hpp>
#include <core/io/file_access.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum PackDirFlags_ { PackDirFlags_Encrypted = 1 << 0 };

	enum PackFileFlags_ { PackFileFlags_Encrypted = 1 << 0 };

	class PackSource;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API PackedData
	{
	public:
		using PathID = size_t;

		struct PackedFile {
			Path pack{};
			u64 offset{}; //if offset is ZERO, the file was ERASED
			u64 size{};
			PathID id{};
			PackSource * src{};
			bool encrypted{};
		};

		struct PackedDir {
			PackedDir * parent{};
			Path name{};
			HashMap<Path, PackedDir *> subdirs{};
			HashSet<Path> files{};
		};

	private:
		static PackedData * __singleton;
		HashMap<Path, PackedFile> m_files{};
		Vector<PackSource *> m_sources{};
		PackedDir * m_root{};
		bool m_enabled{ true };

		void _free_packed_dirs(PackedDir * dir);

	public:
		PackedData();
		~PackedData();
		FORCE_INLINE static PackedData * get_singleton() noexcept { return __singleton; }
#define PACKED_DATA (ism::PackedData::get_singleton())

		Error_ add_pack(Path const & path, bool replace_files, u64 offset);
		void add_pack_source(PackSource * source);
		void add_path(Path const & package_path, Path const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted = false);

		NODISCARD bool is_enabled() const { return m_enabled; }
		void set_enabled(bool enabled) { m_enabled = enabled; }

		NODISCARD Ref<FileAccess> try_open_path(Path const & path);
		NODISCARD bool has_path(Path const & path);

		NODISCARD Ref<DirAccess> try_open_directory(Path const & path);
		NODISCARD bool has_directory(Path const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API PackSource {
	public:
		virtual ~PackSource() noexcept = default;
		virtual bool try_open_pack(Path const & path, bool replace_files, u64 offset) = 0;
		virtual Ref<FileAccess> get_file(Path const & path, PackedData::PackedFile * file) = 0;
	};

	class ISM_API PackedSourcePCK : public PackSource {
	public:
		virtual bool try_open_pack(Path const & path, bool replace_files, u64 offset) override;
		virtual Ref<FileAccess> get_file(Path const & path, PackedData::PackedFile * file) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API FileAccessPack : public Object
	{
		DEFINE_CLASS(FileAccessPack, Object);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API DirAccessPack : public Object
	{
		DEFINE_CLASS(DirAccessPack, Object);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_ACCESS_PACK_HPP_
