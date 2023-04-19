-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
project			"mono"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
targetdir		"%{wks.location}/temps/lib/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
objdir			"%{wks.location}/temps/obj/%{_TARGET_OS}/"
location		"%{wks.location}/workspace/%{_ACTION}/%{prj.name}/"

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
	"{COPYFILE} %{wks.location}/modules/mono/mono.ini %{wks.location}/build/data/",
	"{COPYFILE} %{LIB_DIR}/mono-2.0-sgen%{DLL} %{cfg.debugdir}",
}

postbuildcommands{
	"{COPYFILE} %{cfg.targetdir}/%{prj.targetname}%{DLL} %{cfg.debugdir}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --