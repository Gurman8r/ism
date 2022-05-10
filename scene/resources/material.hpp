#ifndef _ISM_MATERIAL_HPP_
#define _ISM_MATERIAL_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Material : public Resource
	{
		OBJECT_COMMON(Material, Resource);

		RID m_material;

	public:
		Material();

		virtual ~Material();

	public:
		template <class T
		> T get(String const & name)
		{
			return {};
		}

	public:
		template <class T
		> void set(String const & name, T const & value)
		{
		}
	};
}

#endif // !_ISM_MATERIAL_HPP_
