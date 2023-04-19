-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- freetype
project			"freetype"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
targetdir		"%{wks.location}/temps/lib/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
objdir			"%{wks.location}/temps/obj/%{_TARGET_OS}/"
location		"%{wks.location}/workspace/%{_ACTION}/modules/%{prj.name}/"

includedirs{
	"%{wks.location}/thirdparty/freetype2/include",
	"%{wks.location}/thirdparty/freetype2/include/freetype",
}

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --