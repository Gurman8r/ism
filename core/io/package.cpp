#include <core/io/package.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PackageManager * PackageManager::__singleton{};

	PackageManager::PackageManager()
	{
		SINGLETON_CTOR();
		m_root = memnew(PackedDir);
		add_package_source(memnew(PackageSourcePCK));
	}
	
	PackageManager::~PackageManager()
	{
		SINGLETON_DTOR();
		for (size_t i{}; i < m_sources.size(); ++i) {
			memdelete(m_sources[i]);
		}
		memdelete(m_root);
	}

	Error_ PackageManager::add_package(String const & path, bool replace_files, u64 offset)
	{
		for (size_t i{}; i < m_sources.size(); ++i) {
			if (m_sources[i]->try_open_pack(path, replace_files, offset)) {
				return Error_OK;
			}
		}

		return Error_Unknown;
	}

	void PackageManager::add_package_source(PackageSource * source)
	{
		if (source != nullptr) {
			m_sources.push_back(source);
		}
	}

	void PackageManager::add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackageSource * src, bool replace_files, bool encrypted)
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

	Ref<File> PackageManager::try_open_path(String const & path)
	{
		if (auto const it{ m_files.find(path) }; (it == m_files.end()) || !it->second.offset) {
			return nullptr;
		}
		else {
			return it->second.src->get_file(path, &it->second);
		}
	}

	bool PackageManager::has_path(String const & path)
	{
		return m_files.contains(path);
	}

	Ref<Directory> PackageManager::try_open_dir(String const & path)
	{
		return nullptr;
	}

	bool PackageManager::has_dir(String const & path)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool PackageSourcePCK::try_open_pack(String const & path, bool replace_files, u64 offset)
	{
		return false;
	}

	Ref<File> PackageSourcePCK::get_file(String const & path, PackageManager::PackedFile * file)
	{
		return Ref<File>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PackageFile, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PackageDir, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}