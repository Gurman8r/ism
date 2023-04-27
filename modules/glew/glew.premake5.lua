-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glew
project			"glew"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{_BUILD_BIN}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_BUILD_OBJ}"
location		"%{_PROJECT}"

debugenvs{ "%{_BUILD_BIN}", }

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

filter{ "configurations:Debug" } postbuildcommands{ "{COPYFILE} %{_VENDOR}/glew32d.dll %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}/glew32d.lib %{cfg.targetdir}", }
filter{ "configurations:Release" } postbuildcommands{ "{COPYFILE} %{_VENDOR}/glew32.dll %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}/glew32.lib %{cfg.targetdir}", }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --