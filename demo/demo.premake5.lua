-- demo
cpp_project_common("Demo", "demo", "ConsoleApp", "%{_BUILD}")

links_graphics()

dependson{ "ism", "demo-CS", "lua", "mono", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "ism", "mono-2.0-sgen", "zip", "zlibstatic", }

defines{
	"MAIN_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{_SLN}demo/demo.premake5.lua",
	"%{_SLN}assets/**.**",
	"%{_PLATFORM}%{_TARGET_OS}/%{_TARGET_OS}_main.cpp",
	"%{_PLATFORM}windows/ism.rc",
	"%{_PLATFORM}windows/ism.ico",
}

prebuildcommands{
	"{COPYFILE} %{_PLATFORM}windows/bin.manifest %{_BUILD_BIN}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-CS
csharp_project_common("Demo", "demo-CS", "SharedLib", "%{_BUILD_BIN}")

links{ "ism-CS" }

files{ "%{_SLN}demo/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --