-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

--WORKSPACE
workspace "ism_%{_ACTION}"
configurations{ "Debug", "Release" }
platforms{ "x86", "x64" }
filter{ "platforms:*32", "platforms:*86" } architecture "x86"
filter{ "platforms:*64" } architecture "x64"
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

startproject "launcher"

solution_items{
	"premake5.lua",
	"premake5-system.lua",
	"README.md",
	"TODO.txt",
	"engine.ini",
	"editor.ini",
	"extensions.cfg",
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

project "*"

--ENVIRONMENT
debugdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
debugenvs{ "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/", }

--PREPROCESSOR
defines{ "_CRT_SECURE_NO_WARNINGS", }
undefines{ "NDEBUG", }

--COMMANDS
prebuildcommands{
	"{MKDIR} %{wks.location}/bin/",
	"{MKDIR} %{wks.location}/bin/%{cfg.platform}/",
	"{MKDIR} %{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/",
}

--INCLUDEDIRS
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
	"%{wks.location}/thirdparty/imgui-node-editor/",
	"%{wks.location}/thirdparty/json/include/",
}

--LIBDIRS
libdirs{
	"%{wks.location}/bin/",
	"%{wks.location}/bin/%{cfg.platform}/",
	"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/",
	"%{wks.location}/bin-lib/",
	"%{wks.location}/bin-lib/%{cfg.platform}/",
	"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/",
	"%{wks.location}/misc/%{_TARGET_OS}/",
	"%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/",
	"%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/",
}

--OPTIMIZER
filter{ "configurations:Debug" } symbols "On"
filter{ "configurations:Release" } optimize "Speed"
filter{}

--GRAPHICS
filter{ "options:gfxapi=opengl" } links{ "opengl32" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glad" } dependson{ "glad" } links{ "glad" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glew", "configurations:Debug" } dependson{ "glew" } links{ "glew32d" }
filter{ "options:gfxapi=opengl", "options:gl_loader=glew", "configurations:Release" } dependson{ "glew" } links{ "glew32" }
filter{ "options:gfxapi=vulkan" } -- NYI --
filter{ "options:gfxapi=directx" } -- NYI --
filter{ "options:gfxapi=software" } -- NYI --
filter{}

--WIN32
filter{ "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" }
filter{ "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
filter{ "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
