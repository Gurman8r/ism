#ifndef _ISM_MATERIAL_HPP_
#define _ISM_MATERIAL_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Material : public Resource
	{
		ISM_OBJECT(Material, Resource);

	public:
		virtual ~Material();

		Material();

	private:

	};
}

#endif // !_ISM_MATERIAL_HPP_
