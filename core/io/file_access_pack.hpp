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

	class ISM_API Packages
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
		static Packages * __singleton;
		bool m_enabled{ true };
		HashMap<String, PackedFile> m_files{};
		Vector<PackSource *> m_sources{};
		PackedDir * m_root{};

	public:
		Packages();
		~Packages();
		FORCE_INLINE static Packages * get_singleton() noexcept { return __singleton; }
#define PACKAGES (ism::Packages::get_singleton())

		NODISCARD bool is_enabled() const { return m_enabled; }
		void set_enabled(bool enabled) { m_enabled = enabled; }

		Error_ add_pack(String const & path, bool replace_files, u64 offset);
		void add_pack_source(PackSource * source);
		void add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted = false);

		NODISCARD Ref<FileAccess> try_open_path(String const & path);
		NODISCARD bool has_path(String const & path);

		NODISCARD Ref<DirAccess> try_open_dir(String const & path);
		NODISCARD bool has_dir(String const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API PackSource {
	public:
		virtual ~PackSource() noexcept = default;
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) = 0;
		virtual Ref<FileAccess> get_file(String const & path, Packages::PackedFile * file) = 0;
	};

	class ISM_API PackSourcePCK : public PackSource {
	public:
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		virtual Ref<FileAccess> get_file(String const & path, Packages::PackedFile * file) override;
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
