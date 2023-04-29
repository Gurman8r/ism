#ifndef _ISM_MATERIAL_LIBRARY_HPP_
#define _ISM_MATERIAL_LIBRARY_HPP_

#include <scene/resources/material.hpp>

namespace ism
{
	class ISM_API MaterialLibrary final
	{
	public:
		static Error_ load_material(Material & material, String const & path);
		static Error_ load_material(Ref<Material> material, String const & path);
	};
}

#endif // !_ISM_MATERIAL_LIBRARY_HPP_
