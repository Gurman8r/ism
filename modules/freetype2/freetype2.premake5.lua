-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- freetype2
project			"freetype2"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{_BUILD_BIN}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

debugenvs{ "%{_BUILD_BIN}", }

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

postbuildcommands{
	"{COPYFILE} %{_VENDOR}/freetype.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/freetype.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --