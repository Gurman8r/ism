-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

--WORKSPACE

workspace "ism_%{_ACTION}"
startproject "launcher"

platforms{ "x86", "x64" }
filter{ "platforms:*32", "platforms:*86" } architecture "x86" _PLATFORM="x86"
filter{ "platforms:*64" } architecture "x64" _PLATFORM="x64"
filter{}

configurations{ "Debug", "Release" }
filter{ "configurations:Debug" } symbols "On" _BUILDCFG="Debug"
filter{ "configurations:Release" } optimize "Speed" _BUILDCFG="Release"
filter{}

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
--include "editor/editor.premake5.lua"

group "Modules"
include "modules/assimp/assimp.premake5.lua"
include "modules/glew/glew.premake5.lua"
include "modules/freetype/freetype.premake5.lua"
include "modules/glfw/glfw.premake5.lua"
include "modules/imgui/imgui.premake5.lua"
include "modules/mono/mono.premake5.lua"
include "modules/lua/lua.premake5.lua"

group "Programs"
include "programs/launcher/launcher.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

--ALL PROJECTS

project "*"

prebuildcommands{
	"{MKDIR} %{wks.location}/build/",
	"{MKDIR} %{wks.location}/build/bin/",
	"{MKDIR} %{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/",
	"{MKDIR} %{wks.location}/build/data/",
	"{MKDIR} %{wks.location}/build/res/",
	"{MKDIR} %{wks.location}/build/user/",
}

debugenvs{
	"{MKDIR} %{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/",
}

libdirs{
	"{MKDIR} %{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/",
	"%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/",
}

includedirs{
	"%{wks.location}/",
	"%{wks.location}/thirdparty/",
	"%{wks.location}/thirdparty/assimp/include/",
	"%{wks.location}/thirdparty/entt/src/",
	"%{wks.location}/thirdparty/freetype2/include/",
	"%{wks.location}/thirdparty/freetype2/include/freetype/",
	"%{wks.location}/thirdparty/gcem/include/",
	"%{wks.location}/thirdparty/glfw/include/",
	"%{wks.location}/thirdparty/imgui/",
	"%{wks.location}/thirdparty/json/include/",
	"%{wks.location}/thirdparty/mono/include/",
}

defines{
	"_CRT_SECURE_NO_WARNINGS",
}

undefines{
	"NDEBUG",
}

filter{ "options:gfxapi=opengl" } links{ "opengl32" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glad" } dependson{ "glad" } links{ "glad" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glew", "configurations:Debug" } dependson{ "glew" } links{ "glew32d" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glew", "configurations:Release" } dependson{ "glew" } links{ "glew32" }
filter{ "options:gfxapi=vulkan" } -- NYI --
filter{ "options:gfxapi=directx" } -- NYI --
filter{ "options:gfxapi=software" } -- NYI --
filter{}

filter{ "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" }
filter{ "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
filter{ "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
