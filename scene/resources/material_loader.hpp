#ifndef _ISM_MATERIAL_LOADER_HPP_
#define _ISM_MATERIAL_LOADER_HPP_

#include <scene/resources/material.hpp>

namespace ism
{
	class ISM_API MaterialLoader final
	{
	public:
		static Error_ load_material(Material & material, Path const & path);
		static Error_ load_material(Ref<Material> material, Path const & path);
	};
}

#endif // !_ISM_MATERIAL_LOADER_HPP_
