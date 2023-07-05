-- demo
cpp_project_common("App", "demo", "ConsoleApp", "%{_BUILD}")

dependson{ "demo-CS", }

mark_as_executable()

links_graphics()

defines{
	"MAIN_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{_PROGRAMS}demo/demo.premake5.lua",
	"%{_PROGRAMS}demo/demo.ico",
	"%{_PROGRAMS}demo/demo.rc",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-CS
csharp_project_common("App", "demo-CS", "SharedLib", "%{_BUILD_BIN}")

links{ "ism-CS" }

files{ "%{_PROGRAMS}demo/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --