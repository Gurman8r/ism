#include <core/io/file_access_pack.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PackedData * PackedData::__singleton{};

	void PackedData::_free_packed_dirs(PackedDir * dir)
	{
		for (auto & [k, v] : dir->subdirs) {
			_free_packed_dirs(v);
		}
		memdelete(dir);
	}

	PackedData::PackedData()
	{
		__singleton = this;
		m_root = memnew(PackedDir);
		add_pack_source(memnew(PackedSourcePCK));
	}
	
	PackedData::~PackedData()
	{
		for (size_t i{}; i < m_sources.size(); ++i) {
			memdelete(m_sources[i]);
		}
		_free_packed_dirs(m_root);
	}

	Error_ PackedData::add_pack(Path const & path, bool replace_files, u64 offset)
	{
		for (size_t i{}; i < m_sources.size(); ++i) {
			if (m_sources[i]->try_open_pack(path, replace_files, offset)) {
				return Error_OK;
			}
		}

		return Error_Unknown;
	}

	void PackedData::add_pack_source(PackSource * source)
	{
		if (source != nullptr) {
			m_sources.push_back(source);
		}
	}

	void PackedData::add_path(Path const & package_path, Path const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted)
	{
		bool const exists{ m_files.contains(path) };

		PackedFile pf{ package_path, offset, size, id, src, encrypted };
		if (!exists || replace_files) {
			m_files[path] = std::move(pf);
		}

		if (!exists)
		{
			PackedDir * cd{ m_root };
			if (path.string().contains('/'))
			{
				Vector<String> const ds{ path.root_directory().string().split('/') };
				for (size_t j = 0; j < ds.size(); j++) {
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
			if (Path filename{ path.filename() }; !filename.empty()) {
				cd->files.insert(filename);
			}
		}
	}

	Ref<FileAccess> PackedData::try_open_path(Path const & path)
	{
		if (auto const it{ m_files.find(path) }; (it == m_files.end()) || !it->second.offset) {
			return nullptr;
		}
		else {
			return it->second.src->get_file(path, &it->second);
		}
	}

	bool PackedData::has_path(Path const & path)
	{
		return m_files.contains(path);
	}

	Ref<DirAccess> PackedData::try_open_directory(Path const & path)
	{
		return nullptr;
	}

	bool PackedData::has_directory(Path const & path)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool PackedSourcePCK::try_open_pack(Path const & path, bool replace_files, u64 offset)
	{
		return false;
	}

	Ref<FileAccess> PackedSourcePCK::get_file(Path const & path, PackedData::PackedFile * file)
	{
		return Ref<FileAccess>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccessPack, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DirAccessPack, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}