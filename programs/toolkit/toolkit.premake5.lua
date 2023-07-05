-- toolkit
cpp_project_common("Editor", "toolkit", "ConsoleApp", "%{_BUILD}")

dependson{ "toolkit-CS", }

links_graphics()

mark_as_executable()

defines{
	"MAIN_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{_PROGRAMS}toolkit/toolkit.premake5.lua",
	"%{_PROGRAMS}toolkit/toolkit.ico",
	"%{_PROGRAMS}toolkit/toolkit.rc",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- toolkit-CS
csharp_project_common("Editor", "toolkit-CS", "SharedLib", "%{_BUILD_BIN}")

links{ "ism-CS" }

files{ "%{_PROGRAMS}toolkit/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --