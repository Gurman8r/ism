-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ism_sdk_%{_ACTION}"
startproject "toolkit"

configurations{ "Debug", "Release" }
filter{ "configurations:Debug" } symbols "On" optimize "Off"
filter{ "configurations:Release" } symbols "Off" optimize "Speed"
filter{}

platforms{ "x86", "x64", }
filter{ "platforms:*32", "platforms:*86" } architecture "x86"
filter{ "platforms:*64" } architecture "x86_64"
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- solution items
solution_items{
	"premake5-system.lua",
	"premake5.lua",
	"README.md",
}

-- engine
include "assets/assets.premake5.lua"
include "ism.premake5.lua"

-- modules
include "modules/assimp/assimp.premake5.lua"
include "modules/freetype2/freetype2.premake5.lua"
include "modules/glew/glew.premake5.lua"
include "modules/glfw/glfw.premake5.lua"
include "modules/imgui/imgui.premake5.lua"
include "modules/zip/zip.premake5.lua"

-- extensions
include "ext/lua/lua.premake5.lua"
include "ext/mono/mono.premake5.lua"

-- apps
include "app/toolkit/toolkit.premake5.lua"
include "app/demo/demo.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- global project settings
project "*"

debugdir "%{_BUILD}"

debugenvs{ "%{_BUILD}" }

libdirs{ "%{_BUILD_BIN}", }

includedirs{
	"%{_SLN}",
	"%{_THIRDPARTY}",
	"%{_THIRDPARTY}assimp/include/",
	"%{_THIRDPARTY}cxxopts/include/",
	"%{_THIRDPARTY}entt/src/",
	"%{_THIRDPARTY}freetype2/include/",
	"%{_THIRDPARTY}freetype2/include/freetype/",
	"%{_THIRDPARTY}gcem/include/",
	"%{_THIRDPARTY}glfw/include/",
	"%{_THIRDPARTY}imgui/",
	"%{_THIRDPARTY}json/include/",
	"%{_THIRDPARTY}lexertk/",
	"%{_THIRDPARTY}lua/",
	"%{_THIRDPARTY}minizip/",
	"%{_THIRDPARTY}mono/include/",
	"%{_THIRDPARTY}zlib/",
}

if _TARGET_OS=="windows" then
	links_win32()
	generate_manifest("platform/windows/bin.manifest")
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --