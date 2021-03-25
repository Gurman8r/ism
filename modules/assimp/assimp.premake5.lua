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
objdir			"%{wks.location}/bin-obj/%{cfg.platform}/%{cfg.buildcfg}/"
location		"%{wks.location}/workspace/%{_ACTION}/modules/%{prj.name}"

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

prebuildcommands{
	COPYFILE("%{wks.location}misc/%{_TARGET_OS}/%{cfg.platform}/%{cfg.buildcfg}/assimp%{DLL}", "%{wks.location}bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --