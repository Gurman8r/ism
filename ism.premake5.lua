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
	-- configs
	"%{wks.location}/ism.premake5.lua",

	-- headers
	"%{wks.location}/core/**.hpp",
	"%{wks.location}/drivers/**.hpp",
	"%{wks.location}/editor/**.hpp",
	"%{wks.location}/main/**.hpp",
	"%{wks.location}/modules/**.hpp",
	"%{wks.location}/platform/**.hpp", "%{wks.location}/platform/%{_TARGET_OS}/**.hpp",
	"%{wks.location}/scene/**.hpp",
	"%{wks.location}/servers/**.hpp",

	-- sources
	"%{wks.location}/core/**.cpp",
	"%{wks.location}/drivers/**.cpp",
	"%{wks.location}/editor/**.cpp",
	"%{wks.location}/main/**.cpp",
	"%{wks.location}/modules/**.cpp",
	"%{wks.location}/platform/**.cpp", "%{wks.location}/platform/%{_TARGET_OS}/**.cpp",
	"%{wks.location}/scene/**.cpp",
	"%{wks.location}/servers/**.cpp",

	-- inlines
	"%{wks.location}/core/**.inl",
	"%{wks.location}/drivers/**.inl",
	"%{wks.location}/editor/**.inl",
	"%{wks.location}/main/**.inl",
	"%{wks.location}/modules/**.inl",
	"%{wks.location}/platform/**.inl", "%{wks.location}/platform/%{_TARGET_OS}/**.inl",
	"%{wks.location}/scene/**.inl",
	"%{wks.location}/servers/**.inl",
}

postbuildcommands{
	COPY_FILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{DLL}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}
