-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
project			"mono"
kind			"SharedLib"
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

dependson{ "ism", }

links{ "assimp%{LIB}", "freetype", "IrrXML", "zlibstatic", "glfw", "imgui", "ism", "mono-2.0-sgen", }

defines{
	"ISM_MOD_API=ISM_API_EXPORT",
}

includedirs{
	"%{wks.location}/thirdparty/mono/include/",
}

srcdirs(
	"%{wks.location}/modules/mono/"
)

files{
	"%{wks.location}/modules/mono/mono.premake5.lua",
}

prebuildcommands{
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.dll %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.pdb %{cfg.targetdir}",
}

postbuildcommands{
	"{COPYFILE} %{wks.location}/modules/mono/mono.ini %{_BUILD_DAT}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --