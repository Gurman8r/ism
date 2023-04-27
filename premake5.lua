-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

--WORKSPACE

workspace "ism_%{_ACTION}"
startproject "ism-launcher"

platforms{ "x86", "x64" }
filter{ "platforms:*32", "platforms:*86" } architecture "x86" _PLATFORM="x86"
filter{ "platforms:*64" } architecture "x64" _PLATFORM="x64"
filter{}

configurations{ "Debug", "Release" }
filter{ "configurations:Debug" } symbols "On" _BUILDCFG="Debug"
filter{ "configurations:Release" } optimize "Speed" _BUILDCFG="Release"
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- STRINGS

_PROJECT="%{wks.location}/workspace/%{_ACTION}/%{prj.name}/"
_BUILD="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
_BUILD_BIN="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/binaries/"
_BUILD_DATA="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/data/"
_BUILD_RES="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/res/"
_BUILD_USER="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/user/"
_BUILD_OBJ="%{wks.location}/temps/obj/%{_TARGET_OS}/"
_THIRDPARTY="%{wks.location}/thirdparty/"
_VENDOR="%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

--FILES

solution_items{
	"engine.ini",
	"editor.ini",
	"extensions.cfg",
	"premake5.lua",
	"premake5-system.lua",
	"README.md",
	"TODO.txt",
}

group ""
include "ism.premake5.lua"
include "launcher/launcher.premake5.lua"

group "Modules"
include "modules/assimp/assimp.premake5.lua"
include "modules/glew/glew.premake5.lua"
include "modules/freetype/freetype.premake5.lua"
include "modules/glfw/glfw.premake5.lua"
include "modules/imgui/imgui.premake5.lua"
include "modules/mono/mono.premake5.lua"
include "modules/lua/lua.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

--ALL PROJECTS

project "*"

prebuildcommands{
	"{MKDIR} %{_BUILD}",
	"{MKDIR} %{_BUILD_BIN}",
	"{MKDIR} %{_BUILD_DATA}",
	"{MKDIR} %{_BUILD_RES}",
	"{MKDIR} %{_BUILD_USER}",
}

libdirs{
	"%{_BUILD_BIN}",
}

includedirs{
	"%{wks.location}",
	"%{_THIRDPARTY}/",
	"%{_THIRDPARTY}/assimp/include/",
	"%{_THIRDPARTY}/entt/src/",
	"%{_THIRDPARTY}/freetype2/include/",
	"%{_THIRDPARTY}/freetype2/include/freetype/",
	"%{_THIRDPARTY}/gcem/include/",
	"%{_THIRDPARTY}/glfw/include/",
	"%{_THIRDPARTY}/imgui/",
	"%{_THIRDPARTY}/json/include/",
	"%{_THIRDPARTY}/mono/include/",
	"%{_THIRDPARTY}/lua/",
}

filter{ "options:gfxapi=opengl" } links{ "opengl32" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glad" } dependson{ "glad" } links{ "glad" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glew", "configurations:Debug" } dependson{ "glew" } links{ "glew32d" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glew", "configurations:Release" } dependson{ "glew" } links{ "glew32" }
filter{ "options:gfxapi=vulkan" } -- NYI --
filter{ "options:gfxapi=directx" } -- NYI --
filter{ "options:gfxapi=software" } -- NYI --
filter{}

filter{ "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" } defines{ "_CRT_SECURE_NO_WARNINGS", } undefines{ "NDEBUG", } --debugenvs{ "PATH=%{wks.location}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}\\bin\\;$(PATH);", }
filter{ "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
filter{ "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
