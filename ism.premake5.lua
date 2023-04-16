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
location		"%{wks.location}/workspace/%{_ACTION}/%{prj.name}/"

dependson{ "assimp", "freetype", "glfw", "imgui", }

links{ "assimp%{LIB}", "IrrXML", "zlibstatic", "freetype%{LIB}", "glfw", "imgui", }

defines{
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"ISM_API=ISM_API_EXPORT",
	"ISM_EDITOR_API=ISM_API_EXPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

srcdirs(
	"%{wks.location}/core/",
	"%{wks.location}/drivers/" .. _OPTIONS["gfxapi"],
	"%{wks.location}/drivers/" .. _TARGET_OS,
	"%{wks.location}/editor/",
	"%{wks.location}/main/",
	"%{wks.location}/platform/" .. _TARGET_OS,
	"%{wks.location}/scene/",
	"%{wks.location}/servers/"
)

files{
	"%{wks.location}/ism.premake5.lua",
	"%{wks.location}/drivers/register_driver_types.hpp",
	"%{wks.location}/drivers/register_driver_types.cpp",
	"%{wks.location}/modules/register_module_types.hpp",
	"%{wks.location}/modules/register_module_types.cpp",
	"%{wks.location}/platform/register_platform_apis.hpp",
	"%{wks.location}/platform/register_platform_apis.cpp",
}

postbuildcommands{
	"{COPY} %{cfg.targetdir}/%{prj.targetname}%{DLL} %{cfg.debugdir}",
}