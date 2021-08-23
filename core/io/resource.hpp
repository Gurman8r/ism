#ifndef _ISM_RESOURCE_HPP_
#define _ISM_RESOURCE_HPP_

#include <core/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Resource : public Object
	{
		ISM_OBJECT(Resource, Object);

	protected:
		Resource() noexcept : Object{ get_class_static() } {}
		
	public:
		virtual ~Resource();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RESOURCE_HPP_
