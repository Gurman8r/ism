#ifndef _ISM_MATERIAL_LIBRARY_HPP_
#define _ISM_MATERIAL_LIBRARY_HPP_

#include <scene/resources/material.hpp>
#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class MaterialFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(MaterialFormatLoader, ResourceFormatLoader);
	public:
		static Error_ load_material(Ref<Material> material, String const & path);
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATERIAL_LIBRARY_HPP_
