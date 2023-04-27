-- launcher
project			"launcher"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
targetdir		"%{wks.location}/temps/lib/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
objdir			"%{wks.location}/temps/obj/%{_TARGET_OS}/"
location		"%{wks.location}/workspace/%{_ACTION}/programs/%{prj.name}/"

dependson{ "ism", "lua", "mono", }

links{ "assimp%{LIB}", "IrrXML", "zlibstatic", "freetype%{LIB}", "glfw", "imgui", "ism", }

defines{
	"MAIN_ENABLED=true",
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"ISM_MOD_API=ISM_API_IMPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/assets/**.**",
	"%{wks.location}/programs/%{prj.name}/**.**",
	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
}

postbuildcommands{
	"{COPYFILE} %{cfg.targetdir}/%{prj.targetname}%{EXE} %{cfg.debugdir}",
}