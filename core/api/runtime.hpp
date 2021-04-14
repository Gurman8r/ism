#ifndef _ISM_RUNTIME_HPP_
#define _ISM_RUNTIME_HPP_

#include <core/api/types.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ThreadState;
	class InterpreterState;
	class RuntimeState;

	ALIAS(ThreadID) std::thread::id;
	inline auto get_this_thread_id() { return std::this_thread::get_id(); }

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct TypeInfo
	{
		TYPE type;

		std::type_info const * rtti;

		size_t type_size, type_align;

		void(*operator_new)(size_t);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ThreadState
	{
	public:
		ThreadState(InterpreterState * interp);
		~ThreadState();

		ThreadState * prev{}, * next{};

		InterpreterState * interp{};

		uint64_t id{};

		ThreadID thread_id{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API InterpreterState
	{
	public:
		InterpreterState(RuntimeState * runtime);
		~InterpreterState();

		InterpreterState * prev{}, * next{};

		ThreadState * tstate_head{};

		RuntimeState * runtime{};

		int64_t id{};

		bool finalizing{};

		DICT dict{}, builtins{}, modules{}, importlib{};
		OBJECT import_func;

		uint64_t tstate_next_id{};
		ThreadID thread_id{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace core
	{
		template <class = void> NODISCARD RuntimeState * get_runtime() { return RuntimeState::singleton; }

		template <class = void> NODISCARD void set_runtime(RuntimeState * value) { RuntimeState::singleton = value; }
	}

	class ISM_API RuntimeState
	{
	private:
		static RuntimeState * singleton;
		friend RuntimeState * core::get_runtime();
		friend void core::set_runtime(RuntimeState *);

	public:
		RuntimeState();
		~RuntimeState();

		bool preinitializing{}, preinitialized{},  initialized{};
		void * finalizing{};

		struct _interpreters
		{
			InterpreterState * head{}, * main{};
			
			int64_t next_id{ -1 };
		}
		interpreters;

		struct _registry
		{
			TypeMap<TypeInfo *> cpp_types{};
			HashMap<TYPE, Vector<TypeInfo *>> core_types{};
			TypeMap<Vector<bool(*)(OBJECT, void *&)>> direct_conversions{};
			HashMap<String, void *> shared_data{};
		}
		registry;

		void(*on_exit)(OBJECT) {};

		ThreadState * tstate_current{};

		ThreadID main_thread{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace core
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD inline bool is_preinitializing() { return get_runtime()->preinitializing; }
		NODISCARD inline bool is_preinitialized() { return get_runtime()->preinitialized; }
		NODISCARD inline bool is_initialized() { return get_runtime()->initialized; }
		NODISCARD inline auto get_finalizing() { return get_runtime()->finalizing; }

		NODISCARD inline auto get_threadstate() { return get_runtime()->tstate_current; }
		NODISCARD inline auto get_interpreter() { return get_threadstate()->interp; }
		NODISCARD inline auto get_main_interpreter() { return get_runtime()->interpreters.main; }
		NODISCARD inline auto get_head_interpreter() { return get_runtime()->interpreters.head; }

		inline void set_threadstate(ThreadState * value) { get_runtime()->tstate_current = value; }
		inline void set_interpreter(InterpreterState * value) { get_threadstate()->interp = value; }
		inline void set_main_interpreter(InterpreterState * value) { get_runtime()->interpreters.main = value; }

		NODISCARD inline auto lookup_interpreter(int64_t id) -> InterpreterState *
		{
			if (id < 0) { return nullptr; }
			auto it{ get_head_interpreter() };
			while (it) { if (id == it->id) { return it; } else { it = it->next; } }
			return nullptr;
		}

		NODISCARD inline auto & get_registry() { return get_runtime()->registry; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class K = String
		> NODISCARD void * get_shared_data(K && key) noexcept
		{
			auto & shared_data{ get_registry().shared_data };

			auto it{ shared_data.find(FWD(key)) };

			return (it != shared_data.end()) ? it->second : nullptr;
		}

		template <class K = String
		> void * set_shared_data(K && key, void * value) noexcept
		{
			auto & shared_data{ get_registry().shared_data };

			return shared_data.insert_or_assign(FWD(key), value).first->second;
		}

		template <class T, class K, class Fn
		> NODISCARD T & get_or_create_shared_data(K && key, Fn && fn) noexcept
		{
			auto & shared_data{ get_registry().shared_data };

			if (auto it{ shared_data.find(FWD(key)) }; it != shared_data.end())
			{
				return *static_cast<T *>(it->second);
			}
			else
			{
				return *static_cast<T *>(shared_data.insert({ FWD(key), std::invoke(FWD(fn)) }).first->second);
			}
		}

		template <class T, class K = String
		> NODISCARD T & get_or_create_shared_data(K && key) noexcept
		{
			return ism::get_or_create_shared_data<T>(FWD(key), []() { return memnew(T); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RUNTIME_HPP_
