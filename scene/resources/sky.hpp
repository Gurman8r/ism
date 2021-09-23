#ifndef _ISM_SKY_HPP_
#define _ISM_SKY_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Sky : public Resource
	{
		OBJ_COMMON(Sky, Resource);

	public:
		virtual ~Sky();

		Sky();

	private:

	};
}

#endif // !_ISM_SKY_HPP_
