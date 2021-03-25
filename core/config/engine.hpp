#ifndef _ISM_ENGINE_HPP_
#define _ISM_ENGINE_HPP_

#include <core/os/main_loop.hpp>

namespace ISM
{
	class ISM_API Engine
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		struct NODISCARD Singleton final
		{
			Identifier	name{};
			void *		addr{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class Main;

		static Engine * singleton;

		Batch<hash_t, Identifier, void *> m_data{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		void add_singleton(Singleton const & value);
		
		void get_singletons(Vector<Singleton> * list);
		
		bool has_singleton(String const & name) const;
		
		void * get_singleton_object(String const & name) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FORCE_INLINE static Engine * get_singleton() { return singleton; }

		Engine() { singleton = this; }

		~Engine() { singleton = nullptr; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	NODISCARD inline Engine & get_engine() { return *CHECK(Engine::get_singleton()); }
}

#endif // !_ISM_ENGINE_HPP_
