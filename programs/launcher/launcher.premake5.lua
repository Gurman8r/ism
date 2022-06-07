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
objdir			"%{wks.location}/bin-obj/%{cfg.platform}/%{cfg.buildcfg}/"
location		"%{wks.location}/workspace/%{_ACTION}/programs/%{prj.name}"

dependson{ "ism", }

links{ "ism", "assimp", "glew", "glfw", "imgui", }

defines{
	"ISM_MAIN=true",
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/assets/**.**",

	"%{wks.location}/programs/%{prj.name}/**.**",

	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
}

postbuildcommands{
	COPY_FILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{EXE}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}