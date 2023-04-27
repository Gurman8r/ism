-- ism
project			"ism"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
rtti			"On"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
targetdir		"%{wks.location}/temps/lib/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
objdir			"%{wks.location}/temps/obj/%{_TARGET_OS}/"
location		"%{wks.location}/workspace/%{_ACTION}/%{prj.name}/"

dependson{ "assimp", "freetype", "glfw", "imgui", }

links{ "assimp%{LIB}", "IrrXML", "zlibstatic", "freetype%{LIB}", "glfw", "imgui", }

defines{
	"ISM_API_EXPORTS",
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"IMGUI_API=ISM_API_IMPORT",
}

srcdirs(
	"%{wks.location}/core/",
	"%{wks.location}/drivers/".._OPTIONS["gfxapi"].."/",
	"%{wks.location}/drivers/%{_TARGET_OS}/",
	"%{wks.location}/editor/",
	"%{wks.location}/main/",
	"%{wks.location}/platform/%{_TARGET_OS}/",
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
	"{COPYFILE} %{wks.location}/engine.ini %{wks.location}/build/data/",
	"{COPYFILE} %{wks.location}/editor.ini %{wks.location}/build/data/",
	"{COPYFILE} %{wks.location}/extensions.cfg %{wks.location}/build/data/",
	"{COPYFILE} %{cfg.targetdir}/%{prj.targetname}%{DLL} %{cfg.debugdir}",
}