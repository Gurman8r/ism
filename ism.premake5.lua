-- ism
project			"ism"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
rtti			"On"
targetname		"%{prj.name}"
debugdir		"%{_BUILD_BIN}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("ism")

debugenvs{ "%{_BUILD_BIN}", }

dependson{ "assimp", "freetype2", "glfw", "imgui", }

links{ "assimp%{LIB}", "IrrXML", "zlibstatic", "freetype", "glfw", "imgui", }

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
	"{COPYFILE} %{wks.location}/engine.ini %{_BUILD_DAT}",
	"{COPYFILE} %{wks.location}/editor.ini %{_BUILD_DAT}",
	"{COPYFILE} %{wks.location}/extensions.cfg %{_BUILD_DAT}",
}