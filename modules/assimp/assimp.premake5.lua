-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assimp
project			"assimp"
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

manifest("assimp")

debugenvs{ "%{_BUILD_BIN}", }

includedirs{
	"%{wks.location}/thirdparty/assimp/include",
}

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

postbuildcommands{
	"{COPYFILE} %{_VENDOR}/assimp.dll %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/assimp.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/IrrXML.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/zlibstatic.lib %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --