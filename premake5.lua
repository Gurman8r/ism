-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- solution
workspace("ism_%{_ACTION}")

startproject("toolkit")

load_configurations{ "Debug", "Release", }

load_platforms{ "x86", "x64", }

solution_items{
	"premake5-system.lua",
	"premake5.lua",
	"README.md",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- modules
include "modules/assimp/assimp.premake5.lua"
include "modules/freetype2/freetype2.premake5.lua"
include "modules/glew/glew.premake5.lua"
include "modules/glfw/glfw.premake5.lua"
include "modules/imgui/imgui.premake5.lua"
include "modules/zip/zip.premake5.lua"

-- engine
include "ism.premake5.lua"
include "assets/assets.premake5.lua"

-- extensions
include "extensions/lua/lua.premake5.lua"
include "extensions/mono/mono.premake5.lua"

-- applications
include "apps/toolkit/toolkit.premake5.lua"
include "apps/demo/demo.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- all projects
project "*"

debugdir "%{_BUILD}"

debugenvs{ "%{_BUILD}" }

libdirs{ "%{_BUILD}/bin", }

includedirs{
	"%{_SLN}",
	"%{_THIRDPARTY}",
	"%{_THIRDPARTY}/assimp/include",
	"%{_THIRDPARTY}/cxxopts/include",
	"%{_THIRDPARTY}/entt/src",
	"%{_THIRDPARTY}/freetype2/include",
	"%{_THIRDPARTY}/freetype2/include/freetype",
	"%{_THIRDPARTY}/gcem/include",
	"%{_THIRDPARTY}/glfw/include",
	"%{_THIRDPARTY}/imgui",
	"%{_THIRDPARTY}/json/include",
	"%{_THIRDPARTY}/lexertk",
	"%{_THIRDPARTY}/lua",
	"%{_THIRDPARTY}/minizip",
	"%{_THIRDPARTY}/mono/include",
	"%{_THIRDPARTY}/zlib",
}

if _TARGET_OS=="windows" then links_win32() end

generate_manifest("platform/".._TARGET_OS.."/bin.manifest")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --