#ifndef _ISM_MATERIAL_HPP_
#define _ISM_MATERIAL_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	API_CLASS(Material) : public Resource
	{
		OBJ_COMMON(Material, Resource);

	public:
		virtual ~Material();

		Material();

	private:

	};
}

#endif // !_ISM_MATERIAL_HPP_
