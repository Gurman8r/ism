#include <core/api/runtime.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StackFrame::StackFrame()
{
}

StackFrame::~StackFrame()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ThreadState::ThreadState(InterpreterState * interp) : interp{ interp }
{
	thread_id = std::this_thread::get_id();
	VERIFY(-1 < (id = ++interp->tstate_next_id));

	prev = interp->tstate_head;
	if (prev) { prev->next = this; }
	interp->tstate_head = this;
	if (!interp->runtime->tstate_current) { interp->runtime->tstate_current = this; }
}

ThreadState::~ThreadState()
{
	if (next) { next->prev = prev; }
	if (prev) { prev->next = next; } else { interp->tstate_head = next; }
	if (this == interp->runtime->tstate_current) { interp->runtime->tstate_current = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

InterpreterState::InterpreterState(RuntimeState * runtime) : runtime{ runtime }
{
	thread_id = std::this_thread::get_id();
	VERIFY(-1 < (id = ++runtime->interpreters.next_id));

	prev = runtime->interpreters.head;
	if (prev) { prev->next = this; }
	runtime->interpreters.head = this;
	if (!runtime->interpreters.main) { runtime->interpreters.main = this; }

	tstate_head = memnew(ThreadState(this));

	dict = DICT(DictObject{});
	builtins = DICT(DictObject{});
	modules = DICT(DictObject{});
	importlib = DICT(DictObject{});
}

InterpreterState::~InterpreterState()
{
	if (exitfunc) { exitfunc(exitmodule); }

	while (tstate_head) {
		auto next{ tstate_head->next };
		memdelete(tstate_head);
		tstate_head = next;
	}
	if (next) { next->prev = prev; }
	if (prev) { prev->next = next; } else { runtime->interpreters.head = next; }
	if (this == runtime->interpreters.main) { runtime->interpreters.main = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RuntimeState * RuntimeState::singleton{};

RuntimeState::RuntimeState()
{
	if (!get_runtime_state()) { set_runtime_state(this); }

	main_thread = std::this_thread::get_id();

	interpreters.head = memnew(InterpreterState(this));
}

RuntimeState::~RuntimeState()
{
	for (auto const & f : exitfuncs) { if (f) { f(); } }

	while (interpreters.head) {
		auto next{ interpreters.head->next };
		memdelete(interpreters.head);
		interpreters.head = next;
	}

	if (this == get_runtime_state()) { set_runtime_state(nullptr); }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */