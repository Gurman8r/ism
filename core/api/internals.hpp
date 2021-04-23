#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/api/types.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD TypeInfo
	{
		TYPE type{};

		std::type_info const * cpptype{};

		size_t type_size, type_align{};

		void(*operator_new)(size_t) {};

		Vector<OBJECT (*)(OBJECT, TYPE)> implicit_conversions;

		Vector<std::pair<std::type_info const *, void * (*)(void *)>> implicit_casts;

		Vector<bool (*)(OBJECT, void *&)> * direct_conversions;

		void * (*module_local_load)(OBJECT, TypeInfo const *) {};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API StackFrame
	{
	public:
		StackFrame();
		~StackFrame();

		StackFrame * back{};

		DICT builtins{}, globals{}, locals{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API ThreadState
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

	class NODISCARD ISM_API InterpreterState
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

	class NODISCARD ISM_API RuntimeState
	{
	private:
		static RuntimeState * singleton;
		friend RuntimeState * get_default_runtime();
		friend void set_default_runtime(RuntimeState *);

	public:
		RuntimeState();
		~RuntimeState();

		bool preinitializing{}, preinitialized{},  initialized{};
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
			TypeMap<TypeInfo *> registered_types_cpp{};
			HashMap<TYPE, Vector<TypeInfo *>> registered_types_core{};
			TypeMap<Vector<bool(*)(OBJECT, void *&)>> direct_conversions{};
			HashMap<String, void *> shared_data{};
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

	NODISCARD inline TypeMap<TypeInfo *> & registered_local_types_cpp()
	{
		static TypeMap<TypeInfo *> locals{};
		return locals;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class K = String
	> NODISCARD void * get_shared_data(K && key) noexcept
	{
		auto & shared_data{ get_default_registry().shared_data };
		auto it{ shared_data.find(FWD(key)) };
		return (it != shared_data.end()) ? it->second : nullptr;
	}

	template <class K = String
	> void * set_shared_data(K && key, void * value) noexcept
	{
		auto & shared_data{ get_default_registry().shared_data };
		return shared_data.insert_or_assign(FWD(key), value).first->second;
	}

	template <class T, class K, class Fn
	> NODISCARD T & get_or_create_shared_data(K && key, Fn && fn) noexcept
	{
		auto & shared_data{ get_default_registry().shared_data };
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
		return get_or_create_shared_data<T>(FWD(key), []() { return memnew(T); });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
