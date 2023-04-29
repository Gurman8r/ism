#include <core/io/file_access_pack.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Packages * Packages::__singleton{};

	Packages::Packages()
	{
		__singleton = this;
		m_root = memnew(PackedDir);
		add_pack_source(memnew(PackSourcePCK));
	}
	
	Packages::~Packages()
	{
		for (size_t i{}; i < m_sources.size(); ++i) {
			memdelete(m_sources[i]);
		}
		memdelete(m_root);
	}

	Error_ Packages::add_pack(String const & path, bool replace_files, u64 offset)
	{
		for (size_t i{}; i < m_sources.size(); ++i) {
			if (m_sources[i]->try_open_pack(path, replace_files, offset)) {
				return Error_OK;
			}
		}

		return Error_Unknown;
	}

	void Packages::add_pack_source(PackSource * source)
	{
		if (source != nullptr) {
			m_sources.push_back(source);
		}
	}

	void Packages::add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted)
	{
		bool const exists{ m_files.contains(path) };

		if ((!exists || replace_files) && !path.filename().empty()) {
			m_files[path] = PackedFile{ package_path, offset, size, id, src, encrypted };
			printf("%s\n", path.c_str());
		}

		if (!exists)
		{
			String p{ path };
			if (size_t i{ p.find("://")}; i != String::npos) {
				p.erase(p.begin(), p.begin() + i + 2);
			}

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

	Ref<FileAccess> Packages::try_open_path(String const & path)
	{
		if (auto const it{ m_files.find(path) }; (it == m_files.end()) || !it->second.offset) {
			return nullptr;
		}
		else {
			return it->second.src->get_file(path, &it->second);
		}
	}

	bool Packages::has_path(String const & path)
	{
		return m_files.contains(path);
	}

	Ref<DirAccess> Packages::try_open_dir(String const & path)
	{
		return nullptr;
	}

	bool Packages::has_dir(String const & path)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool PackSourcePCK::try_open_pack(String const & path, bool replace_files, u64 offset)
	{
		return false;
	}

	Ref<FileAccess> PackSourcePCK::get_file(String const & path, Packages::PackedFile * file)
	{
		return Ref<FileAccess>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccessPack, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DirAccessPack, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}