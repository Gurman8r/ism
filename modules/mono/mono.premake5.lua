-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
project			"mono"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/"
location		"%{wks.location}/workspace/%{_ACTION}/modules/%{prj.name}/"

dependson{ "ism", }

links{ "assimp%{LIB}", "freetype%{LIB}", "IrrXML", "zlibstatic", "glfw", "imgui", "ism", "mono-2.0-sgen", }

defines{
	"ISM_API=ISM_API_IMPORT",
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
	"{COPY} %{LIB_DIR}/mono-2.0-sgen%{DLL} %{cfg.debugdir}",
}

postbuildcommands{
	"{COPY} %{cfg.targetdir}/%{prj.targetname}%{DLL} %{cfg.debugdir}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --