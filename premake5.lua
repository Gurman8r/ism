-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ism_%{_ACTION}"
startproject "ism-launcher"

platforms{ "x86", "x64" }
filter{ "platforms:*32", "platforms:*86" } architecture "x86"
filter{ "platforms:*64" } architecture "x64"
filter{}

configurations{ "Debug", "Release" }
filter{ "configurations:Debug" } symbols "On" optimize "Off"
filter{ "configurations:Release" } symbols "Off" optimize "Speed"
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

_BUILD="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
_BUILD_BIN="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/bin/"
_BUILD_DAT="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/dat/"
_BUILD_ETC="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/etc/"
_BUILD_LIB="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/lib/"
_BUILD_RES="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/res/"
_BUILD_USR="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/usr/"
_PROJECT="%{wks.location}/workspace/%{_ACTION}/%{prj.name}/"
_TEMPS="%{wks.location}/temps/obj/%{_ACTION}/%{_TARGET_OS}/"
_THIRDPARTY="%{wks.location}/thirdparty/"
_VENDOR="%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

solution_items{
	"README.md",
	"TODO.txt",
	"engine.ini",
	"editor.ini",
	"extensions.cfg",
	"premake5.lua",
	"premake5-system.lua",
}

group ""
include "ism.premake5.lua"
include "launcher/launcher.premake5.lua"

group "Modules"
include "modules/assimp/assimp.premake5.lua"
include "modules/glew/glew.premake5.lua"
include "modules/freetype2/freetype2.premake5.lua"
include "modules/glfw/glfw.premake5.lua"
include "modules/imgui/imgui.premake5.lua"
include "modules/mono/mono.premake5.lua"
include "modules/lua/lua.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "*"

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

prebuildcommands{
	"{MKDIR} %{_BUILD}",
	"{MKDIR} %{_BUILD_BIN}",
	"{MKDIR} %{_BUILD_DAT}",
	"{MKDIR} %{_BUILD_ETC}",
	"{MKDIR} %{_BUILD_LIB}",
	"{MKDIR} %{_BUILD_RES}",
	"{MKDIR} %{_BUILD_USR}",
}

filter{ "options:gfxapi=opengl" } links{ "opengl32" } defines{ "OPENGL_ENABLED=true" }
filter{ "options:gfxapi=opengl", "options:glapi=glad" } dependson{ "glad" } links{ "glad" } defines{ "OPENGL_LOADER_GLAD=true" }
filter{ "options:gfxapi=opengl", "options:glapi=glew" } dependson{ "glew" } defines{ "OPENGL_LOADER_GLEW=true" }
filter{ "options:gfxapi=opengl", "options:glapi=glew", "configurations:Debug" } links{ "glew32d" }
filter{ "options:gfxapi=opengl", "options:glapi=glew", "configurations:Release" } links{ "glew32" }
filter{ "options:gfxapi=vulkan" } -- NYI --
filter{ "options:gfxapi=directx" } -- NYI --
filter{ "options:gfxapi=software" } -- NYI --
filter{}

filter{ "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" } defines{ "_CRT_SECURE_NO_WARNINGS" } undefines{ "NDEBUG" }
filter{ "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
filter{ "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

generate_manifest("launcher/bin.manifest")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --