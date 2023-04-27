#ifndef _ISM_PACK_HPP_
#define _ISM_PACK_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API PackedData : public Object
	{
		DEFINE_CLASS(PackedData, Object);

	protected:
		PackedData() noexcept {}

	public:
		virtual ~PackedData() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PACK_HPP_
