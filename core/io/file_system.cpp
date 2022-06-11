#include <core/io/file_system.hpp>

#include <filesystem>

using namespace ism;

bool FileSystem::get_file_contents(DynamicBuffer & out, Path const & path)
{
	std::ifstream file{ path.c_str(), std::ios_base::binary };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return false; }
	file.seekg(0, std::ios_base::end);
	if (std::streampos size{ file.tellg() }; 0 < size) {
		file.seekg(0, std::ios_base::beg);
		out.resize((size_t)size);
		file.read((char *)out.data(), (std::streamsize)size);
	}
	return true;
}

Optional<DynamicBuffer> FileSystem::get_file_contents(Path const & path)
{
	if (DynamicBuffer buffer; get_file_contents(buffer, path))
	{
		return make_optional(buffer);
	}
	else
	{
		return nullopt;
	}
}