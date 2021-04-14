#include <core/api/runtime.hpp>
#include <core/api/bind.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ThreadState::ThreadState(InterpreterState * interp) : interp{ interp }
{
	thread_id = get_this_thread_id();

	id = ++interp->tstate_next_id;

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
	thread_id = get_this_thread_id();

	id = ++runtime->interpreters.next_id;
	VERIFY(-1 < id);

	prev = runtime->interpreters.head;
	if (prev) { prev->next = this; }
	runtime->interpreters.head = this;
	if (!runtime->interpreters.main) { runtime->interpreters.main = this; }

	tstate_head = memnew(ThreadState(this));

	dict = DICT::create();
	builtins = DICT::create();
	modules = DICT::create();
	importlib = DICT::create();
}

InterpreterState::~InterpreterState()
{
	if (next) { next->prev = prev; }
	if (prev) { prev->next = next; } else { runtime->interpreters.head = next; }
	if (this == runtime->interpreters.main) { runtime->interpreters.main = nullptr; }

	while (tstate_head) {
		auto next{ tstate_head->next };
		memdelete(tstate_head);
		tstate_head = next;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RuntimeState * RuntimeState::singleton{};

RuntimeState::RuntimeState()
{
	if (!core::get_runtime()) { core::set_runtime(this); }

	main_thread = get_this_thread_id();

	interpreters.head = memnew(InterpreterState(this));
}

RuntimeState::~RuntimeState()
{
	while (interpreters.head) {
		auto next{ interpreters.head->next };
		memdelete(interpreters.head);
		interpreters.head = next;
	}

	if (this == core::get_runtime()) { core::set_runtime(nullptr); }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */