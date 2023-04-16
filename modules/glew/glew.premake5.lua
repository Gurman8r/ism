-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glew
project			"glew"
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

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

filter{ "configurations:Debug" } prebuildcommands{ "{COPY} %{LIB_DIR}/glew32d%{DLL} %{cfg.debugdir}", }
filter{ "configurations:Release" } prebuildcommands{ "{COPY} %{LIB_DIR}/glew32%{DLL} %{cfg.debugdir}", }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --