#include <core/config/engine.hpp>

namespace ISM
{
	Engine * Engine::singleton{};

	void Engine::add_singleton(Singleton const & value)
	{
		auto const id{ value.name.hash_code() };

		if (!m_data.contains<hash_t>(id))
		{
			m_data.push_back(id, value.name, value.addr);
		}
	}

	void Engine::get_singletons(Vector<Singleton> * list)
	{
		if (!list) { return; }

		for (size_t i = 0; i < m_data.size(); ++i)
		{
			m_data.expand_all(i, [&](auto & id, auto & name, auto & addr)
			{
				list->push_back(Singleton{ name, addr });
			});
		}
	}

	bool Engine::has_singleton(String const & name) const
	{
		return m_data.contains<hash_t>(hashof(name));
	}

	void * Engine::get_singleton_object(String const & name) const
	{
		return *m_data.map<hash_t, void *>(hashof(name));
	}
}