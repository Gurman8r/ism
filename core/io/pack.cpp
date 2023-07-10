#include <core/io/pack.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_EMBED(PackedData);

	PackedData::PackedData()
	{
		SINGLETON_CTOR();
		m_root = memnew(PackedDir);
		add_package_source(memnew(PackSourcePAK));
	}
	
	PackedData::~PackedData()
	{
		SINGLETON_DTOR();
		for (size_t i{}; i < m_sources.size(); ++i) {
			memdelete(m_sources[i]);
		}
		memdelete(m_root);
	}

	Error_ PackedData::add_package(String const & path, bool replace_files, u64 offset)
	{
		for (size_t i{}; i < m_sources.size(); ++i) {
			if (m_sources[i]->try_open_pack(path, replace_files, offset)) {
				return Error_OK;
			}
		}

		return Error_Failed;
	}

	void PackedData::add_package_source(PackSource * source)
	{
		if (source != nullptr) {
			m_sources.push_back(source);
		}
	}

	void PackedData::add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted)
	{
		bool const exists{ m_files.contains(path) };

		if ((!exists || replace_files) && !path.filename().empty()) {
			m_files[path] = PackedFile{ package_path, offset, size, id, src, encrypted };
			printf("%s\n", path.c_str());
		}

		if (!exists)
		{
			String p{ path };
			PRINT_LINE(p.root_directory());
			if (size_t i{ p.find("://")}; i != p.npos) { p.erase(p.begin(), p.begin() + i + 2); }

			PackedDir * cd{ m_root };
			if (p.contains('/'))
			{
				Vector<String> const ds{ p.root_directory().split('/') };
				for (size_t j{}; j < ds.size(); ++j) {
					if (!cd->subdirs.contains(ds[j])) {
						PackedDir * pd{ memnew(PackedDir) };
						pd->name = ds[j];
						pd->parent = cd;
						cd->subdirs[pd->name] = pd;
						cd = pd;
					}
					else {
						cd = cd->subdirs[ds[j]];
					}
				}
			}
			if (String filename{ path.filename() }) {
				cd->files.insert(filename);
			}
		}
	}

	Ref<File> PackedData::try_open_path(String const & path)
	{
		if (auto const it{ m_files.find(path) }; (it == m_files.end()) || !it->second.offset) {
			return nullptr;
		}
		else {
			return it->second.src->get_file(path, &it->second);
		}
	}

	bool PackedData::has_path(String const & path)
	{
		return m_files.contains(path);
	}

	Ref<Dir> PackedData::try_open_dir(String const & path)
	{
		return nullptr;
	}

	bool PackedData::has_dir(String const & path)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool PackSourcePAK::try_open_pack(String const & path, bool replace_files, u64 offset)
	{
		return false;
	}

	Ref<File> PackSourcePAK::get_file(String const & path, PackedData::PackedFile * file)
	{
		return Ref<File>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(PackFile, t) {}

	Error_ PackFile::open_internal(String const & path, FileMode_ mode)
	{
		return Error_();
	}

	PackFile::PackFile(String const & path, PackedData::PackedFile const & file) {}

	bool PackFile::is_open() const
	{
		return false;
	}

	PackFile & PackFile::seek(u64 position)
	{
		return (*this);
	}

	PackFile & PackFile::seek_end(int64_t position)
	{
		return (*this);
	}

	u64 PackFile::get_position() const
	{
		return u64();
	}

	u64 PackFile::get_length() const
	{
		return u64();
	}

	bool PackFile::eof_reached() const
	{
		return false;
	}

	u8 PackFile::get_8() const
	{
		return u8();
	}

	u64 PackFile::get_buffer(u8 * data, u64 size) const
	{
		return u64();
	}

	void PackFile::set_big_endian(bool big_endian) noexcept
	{
	}

	Error_ PackFile::get_error() const
	{
		return Error_();
	}

	PackFile & PackFile::flush()
	{
		return (*this);
	}

	PackFile & PackFile::put_8(u8 p_dest)
	{
		return (*this);
	}

	PackFile & PackFile::put_buffer(u8 const * data, u64 size)
	{
		return (*this);
	}

	bool PackFile::file_exists(String const & name)
	{
		return false;
	}

	PackFile & PackFile::close()
	{
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(PackDir, t) {}

	PackDir::PackDir() {}

	PackedData::PackedDir * PackDir::_find_dir(String path)
	{
		return nullptr;
	}

	Error_ PackDir::list_dir_begin()
	{
		return Error_();
	}

	String PackDir::get_next()
	{
		return String();
	}

	bool PackDir::current_is_dir() const
	{
		return false;
	}

	bool PackDir::current_is_hidden() const
	{
		return false;
	}

	void PackDir::list_dir_end()
	{
	}

	i32 PackDir::get_drive_count()
	{
		return i32();
	}

	String PackDir::get_drive(i32 drive)
	{
		return String();
	}

	Error_ PackDir::change_dir(String path)
	{
		return Error_();
	}

	String PackDir::get_current_dir(bool include_drive) const
	{
		return String();
	}

	bool PackDir::file_exists(String path)
	{
		return false;
	}

	bool PackDir::dir_exists(String path)
	{
		return false;
	}

	Error_ PackDir::make_dir(String path)
	{
		return Error_();
	}

	Error_ PackDir::rename(String from, String to)
	{
		return Error_();
	}

	Error_ PackDir::remove(String p_name)
	{
		return Error_();
	}

	u64 PackDir::get_space_left()
	{
		return u64();
	}

	String PackDir::get_filesystem_type() const
	{
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}