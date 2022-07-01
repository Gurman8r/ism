#ifndef _ISM_MATERIAL_LOADER_HPP_
#define _ISM_MATERIAL_LOADER_HPP_

#include <scene/resources/material.hpp>

namespace ism
{
	class ISM_API MaterialLoader final
	{
	public:
		/* load from file */
		static Error_ load_from_file(Material & material, Path const & path);
		static Error_ load_from_file(Ref<Material> material, Path const & path);
	};
}

#endif // !_ISM_MATERIAL_LOADER_HPP_
