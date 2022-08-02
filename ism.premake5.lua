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
objdir			"%{wks.location}/bin-obj/"
location		"%{wks.location}/workspace/%{_ACTION}/%{prj.name}"

dependson{ "assimp", "glew", "glfw", "imgui", }

links{ "assimp.lib", "IrrXML", "zlibstatic", "glew", "glfw", "imgui" }

defines{
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"ISM_API=ISM_API_EXPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/ism.premake5.lua",

	"%{wks.location}/core/**.hpp", "%{wks.location}/core/**.cpp", "%{wks.location}/core/**.inl",

	"%{wks.location}/drivers/**.hpp", "%{wks.location}/drivers/**.cpp", "%{wks.location}/drivers/**.inl",

	"%{wks.location}/editor/**.hpp", "%{wks.location}/editor/**.cpp", "%{wks.location}/editor/**.inl",

	"%{wks.location}/main/**.hpp", "%{wks.location}/main/**.cpp", "%{wks.location}/main/**.inl",

	"%{wks.location}/platform/*.hpp", "%{wks.location}/platform/*.cpp", "%{wks.location}/platform/*.inl",

	"%{wks.location}/platform/%{_TARGET_OS}/**.hpp", "%{wks.location}/platform/%{_TARGET_OS}/**.cpp", "%{wks.location}/platform/%{_TARGET_OS}/**.inl",

	"%{wks.location}/scene/**.hpp", "%{wks.location}/scene/**.cpp", "%{wks.location}/scene/**.inl",

	"%{wks.location}/servers/**.hpp", "%{wks.location}/servers/**.cpp", "%{wks.location}/servers/**.inl",
}

postbuildcommands{
	COPY_FILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{DLL}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}
