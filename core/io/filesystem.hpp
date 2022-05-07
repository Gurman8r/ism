#ifndef _ISM_FILESYSTEM_HPP_
#define _ISM_FILESYSTEM_HPP_

#include <core/os/os.hpp>
#include <fstream>

namespace ism
{
	inline Optional<DynamicBuffer> get_file_contents(Path const & path)
	{
		std::ifstream file{ path.c_str(), std::ios_base::binary };
		SCOPE_EXIT(&file) { file.close(); };
		if (!file) { return nullopt; }

		DynamicBuffer temp;
		file.seekg(0, std::ios_base::end);
		if (int64_t size{ file.tellg() }; 0 < size)
		{
			file.seekg(0, std::ios_base::beg);
			temp.resize((size_t)size);
			file.read((char *)temp.data(), size);
		}
		return temp;
	}
}

#endif // !_ISM_FILESYSTEM_HPP_
