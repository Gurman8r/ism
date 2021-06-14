-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

include "./misc/solution_items.lua"

DLL = ".dll"
EXE = ".exe"
function COPYFILE(src, dst)
	if _TARGET_OS == "windows" then
		return "xcopy /y " .. path.translate(src, "\\") .. " " .. path.translate(dst, "\\") .. "*"
	else
		return "cp -rf " .. path.translate(src, "/") .. " " .. path.translate(dst, "/") .. " "
	end
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ism_api_%{_ACTION}"

startproject "sandbox"

configurations{ "Debug", "Release" }

platforms{ "x86", "x64" }

filter{ "platforms:*32", "platforms:*86" } architecture "x86"
filter{ "platforms:*64" } architecture "x64"
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

solution_items{
	".ism",
	"premake5.lua",
	"README.md",
}

group ""
	include "ism.premake5.lua"

group "Modules"
	include "modules/assimp/assimp.premake5.lua"
	include "modules/python/python.premake5.lua"
	include "modules/glew/glew.premake5.lua"
	include "modules/glfw/glfw.premake5.lua"
	include "modules/imgui/imgui.premake5.lua"

group "Programs"
	include "programs/sandbox/sandbox.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "*"

-- Include Directories
includedirs{
	"%{wks.location}",
	"%{wks.location}/source",
	"%{wks.location}/thirdparty",
	"%{wks.location}/thirdparty/assimp/include",
	"%{wks.location}/thirdparty/glfw/include",
	"%{wks.location}/thirdparty/freetype2/include",
	"%{wks.location}/thirdparty/freetype2/include/freetype",
	"%{wks.location}/thirdparty/json/include",
	"%{wks.location}/thirdparty/entt/src",
	"%{wks.location}/thirdparty/imgui",
	"%{wks.location}/thirdparty/imgui-node-editor",
}

-- Preprocessor
defines{ "_CRT_SECURE_NO_WARNINGS", }
undefines{ "NDEBUG", }

-- Environment
debugdir("%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/")
debugenvs{ "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/", }

-- Optimization
filter{ "configurations:Debug" } symbols "On"
filter{ "configurations:Release" } optimize "Speed"
filter{}

-- Linker
libdirs{
	"%{wks.location}/bin-lib/",
	"%{wks.location}/bin-lib/%{cfg.platform}/",
	"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/",
	"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/modules/",
	"%{wks.location}/misc/%{_TARGET_OS}/",
	"%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/",
	"%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/",
	"%{wks.location}/misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/modules/",
}
links{ "opengl32", "freetype", "assimp", "IrrXML", "zlibstatic", }
filter{ "configurations:Debug" } links{ "glew32d", "python39_d", }
filter{ "configurations:Release" } links{ "glew32", "python39", }
filter{ "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
filter{ "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --