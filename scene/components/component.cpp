#include <scene/components/component.hpp>

using namespace ism;

EMBEDED_CLASS(Component, t)
{
	t.tp_dictoffset = offsetof(Component, m_dict);

	CLASS_DEF(Component, t)
	{
		return t
			.def_readonly("__dict__", &Component::m_dict)
			;
	};
}

Component::Component(Entity * entity) noexcept : m_entity{ VALIDATE(entity) }
{
}

Component::~Component() noexcept
{
	m_entity = nullptr;
}