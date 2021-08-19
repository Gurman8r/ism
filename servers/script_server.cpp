#include <servers/script_server.hpp>

using namespace ism;

DECLEXPR(ism::ScriptServer::singleton) {};

ScriptServer::ScriptServer()
{
	singleton = this;

	dict = DICT::new_();
	
	builtins = DICT::new_();
	
	modules = DICT::new_();
	
	importlib = DICT::new_();

	import_func = nullptr;
}

ScriptServer::~ScriptServer()
{
}