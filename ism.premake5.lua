-- ism
project			"ism"
kind			"SharedLib"
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

links{ "assimp", "glew", "glfw", "imgui" }

defines{
	"TOOLS_ENABLED",
	"OPENGL_ENABLED",
	"OPENGL_LOADER_GLEW",
	"ISM_API=ISM_API_EXPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/ism.premake5.lua",

	"%{wks.location}/core/**.hpp", "%{wks.location}/core/**.cpp",

	"%{wks.location}/drivers/**.hpp", "%{wks.location}/drivers/**.cpp",

	"%{wks.location}/editor/**.hpp", "%{wks.location}/editor/**.cpp",

	"%{wks.location}/main/**.hpp", "%{wks.location}/main/**.cpp",

	"%{wks.location}/platform/*.hpp", "%{wks.location}/platform/*.cpp",
	"%{wks.location}/platform/%{_TARGET_OS}/**.hpp", "%{wks.location}/platform/%{_TARGET_OS}/**.cpp",

	"%{wks.location}/runtime/**.hpp", "%{wks.location}/runtime/**.cpp",
}

postbuildcommands{
	COPYFILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{DLL}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}
