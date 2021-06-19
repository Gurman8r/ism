#ifndef _ISM_RUNTIME_HPP_
#define _ISM_RUNTIME_HPP_

#include <core/api/types.hpp>

namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API StackFrame
	{
	public:
		StackFrame();
		~StackFrame();

		StackFrame * back{};

		DICT builtins{}, globals{}, locals{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ThreadState
	{
	public:
		ThreadState(InterpreterState * interp);
		~ThreadState();

		ThreadState * prev{}, * next{};

		InterpreterState * interp{};

		StackFrame * frame{};

		int64_t id{};

		std::thread::id thread_id{};
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
		OBJECT import_func{};

		void(*exitfunc)(OBJECT) {};
		OBJECT exitmodule{};

		int64_t tstate_next_id{ -1 };

		std::thread::id thread_id{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RuntimeState
	{
	private:
		static RuntimeState * singleton;
		friend RuntimeState * get_default_runtime();
		friend void set_default_runtime(RuntimeState *);

	public:
		RuntimeState();
		~RuntimeState();

		bool preinitializing{}, preinitialized{}, initialized{};
		void * finalizing{};

		std::thread::id main_thread{};
		Vector<void(*)()> exitfuncs{};
		ThreadState * tstate_current{};

		struct _interpreters
		{
			InterpreterState * head{}, * main{};
			int64_t next_id{ -1 };
		}
		interpreters;

		struct _registry
		{
			TypeMap<Vector<bool(*)(OBJECT, void *&)>> direct_conversions{};
		}
		registry;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_default_runtime() -> RuntimeState * { return RuntimeState::singleton; }
	
	NODISCARD inline void set_default_runtime(RuntimeState * value) { RuntimeState::singleton = value; }
	
	NODISCARD inline auto get_default_registry() -> auto & { return get_default_runtime()->registry; }
	
	NODISCARD inline auto get_default_thread() { return get_default_runtime()->tstate_current; }
	
	NODISCARD inline auto get_default_interpreter() { return get_default_thread()->interp; }
	
	NODISCARD inline auto get_default_frame() { return get_default_thread()->frame; }
	
	NODISCARD inline auto get_head_interpreter() { return get_default_runtime()->interpreters.head; }
	
	NODISCARD inline auto get_main_interpreter() { return get_default_runtime()->interpreters.main; }
	
	inline void set_main_interpreter(InterpreterState * value) { get_default_runtime()->interpreters.main = value; }

	NODISCARD inline auto lookup_interpreter(int64_t id) -> InterpreterState *
	{
		if (id < 0) { return nullptr; }
		auto it{ get_head_interpreter() };
		while (it) { if (id == it->id) { return it; } else { it = it->next; } }
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RUNTIME_HPP_
