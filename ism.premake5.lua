-- ism
project			"ism"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
rtti			"On"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/%{cfg.platform}/%{cfg.buildcfg}/"
location		"%{wks.location}/workspace/%{_ACTION}/%{prj.name}"

dependson{ "assimp", "glew", "glfw", "imgui", }

links{ "assimp.lib", "IrrXML", "zlibstatic", "glew", "glfw", "imgui" }

defines{
	"TOOLS_ENABLED=1",
	"OPENGL_ENABLED=1",
	"OPENGL_LOADER_GLEW=1",
	"ISM_API=ISM_API_EXPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/ism.premake5.lua",

	"%{wks.location}/assets/**.**",

	"%{wks.location}/core/**.hpp", "%{wks.location}/core/**.cpp",

	"%{wks.location}/drivers/**.hpp", "%{wks.location}/drivers/**.cpp",

	"%{wks.location}/editor/**.hpp", "%{wks.location}/editor/**.cpp",

	"%{wks.location}/main/**.hpp", "%{wks.location}/main/**.cpp",

	"%{wks.location}/platform/*.hpp", "%{wks.location}/platform/*.cpp",

	"%{wks.location}/platform/%{_TARGET_OS}/**.hpp", "%{wks.location}/platform/%{_TARGET_OS}/**.cpp",

	"%{wks.location}/scene/**.hpp", "%{wks.location}/scene/**.cpp",

	"%{wks.location}/servers/**.hpp", "%{wks.location}/servers/**.cpp",
}

postbuildcommands{
	COPY_FILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{EXE}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}
