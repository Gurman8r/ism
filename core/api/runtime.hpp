#ifndef _ISM_RUNTIME_HPP_
#define _ISM_RUNTIME_HPP_

#include <core/api/types.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace core {}

	class FrameState;
	class InterpreterState;
	class RuntimeState;
	class ThreadState;

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

	struct TypeInfo
	{
		TYPE type;

		std::type_info const * rtti;

		size_t type_size, type_align;

		void(*operator_new)(size_t);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API FrameState
	{
	public:
		FrameState();
		~FrameState();

		FrameState * back{};

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

		FrameState * frame{};

		uint64_t id{};

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
		OBJECT import_func;

		void(*exitfunc)(OBJECT) {};
		OBJECT exitmodule{};

		uint64_t tstate_next_id{};

		std::thread::id thread_id{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RuntimeState
	{
	private:
		static RuntimeState * singleton;
		friend RuntimeState * get_runtime();
		friend void set_runtime(RuntimeState *);

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

		std::thread::id main_thread{};

		Vector<void(*)()> exitfuncs{};

		ThreadState * tstate_current{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_runtime() -> RuntimeState * { return RuntimeState::singleton; }
	NODISCARD inline void set_runtime(RuntimeState * value) { RuntimeState::singleton = value; }

	NODISCARD inline auto get_threadstate() { return get_runtime()->tstate_current; }
	NODISCARD inline auto get_interpstate() { return get_threadstate()->interp; }
	NODISCARD inline auto get_framestate() { return get_threadstate()->frame; }

	NODISCARD inline auto get_head_interpreter() { return get_runtime()->interpreters.head; }
	NODISCARD inline auto get_main_interpreter() { return get_runtime()->interpreters.main; }
	inline void set_main_interpreter(InterpreterState * value) { get_runtime()->interpreters.main = value; }
	
	NODISCARD inline auto lookup_interpreter(int64_t id) -> InterpreterState *
	{
		if (id < 0) { return nullptr; }
		auto it{ get_head_interpreter() };
		while (it) { if (id == it->id) { return it; } else { it = it->next; } }
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class K = String
	> NODISCARD void * get_shared_data(K && key) noexcept
	{
		auto & shared_data{ get_runtime()->registry.shared_data };
		auto it{ shared_data.find(FWD(key)) };
		return (it != shared_data.end()) ? it->second : nullptr;
	}

	template <class K = String
	> void * set_shared_data(K && key, void * value) noexcept
	{
		auto & shared_data{ get_runtime()->registry.shared_data };
		return shared_data.insert_or_assign(FWD(key), value).first->second;
	}

	template <class T, class K, class Fn
	> NODISCARD T & get_or_create_shared_data(K && key, Fn && fn) noexcept
	{
		auto & shared_data{ get_runtime()->registry.shared_data };
		if (auto it{ shared_data.find(FWD(key)) }; it != shared_data.end()) {
			return *static_cast<T *>(it->second);
		}
		else {
			return *static_cast<T *>(shared_data.insert({ FWD(key), std::invoke(FWD(fn)) }).first->second);
		}
	}

	template <class T, class K = String
	> NODISCARD T & get_or_create_shared_data(K && key) noexcept
	{
		return get_or_create_shared_data<T>(FWD(key), []() { return memnew(T); });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RUNTIME_HPP_
