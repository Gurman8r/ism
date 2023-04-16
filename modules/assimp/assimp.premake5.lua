-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assimp
project			"assimp"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/"
location		"%{wks.location}/workspace/%{_ACTION}/modules/%{prj.name}/"

includedirs{
	"%{wks.location}/thirdparty/assimp/include",
}

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

prebuildcommands{
	"{COPY} %{LIB_DIR}/assimp%{DLL} %{cfg.debugdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --