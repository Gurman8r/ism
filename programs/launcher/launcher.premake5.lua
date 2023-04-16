-- launcher
project			"launcher"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/"
location		"%{wks.location}/workspace/%{_ACTION}/programs/%{prj.name}/"

dependson{ "ism", "mono", }

links{ "assimp%{LIB}", "IrrXML", "zlibstatic", "freetype%{LIB}", "glfw", "imgui", "ism", }

defines{
	"MAIN_ENABLED=true",
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"ISM_API=ISM_API_IMPORT",
	"ISM_MOD_API=ISM_API_IMPORT",
	"ISM_EDITOR_API=ISM_API_IMPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/assets/**.**",
	"%{wks.location}/programs/%{prj.name}/**.**",
	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
}

postbuildcommands{
	"{COPY} %{cfg.targetdir}/%{prj.targetname}%{EXE} %{cfg.debugdir}",
}