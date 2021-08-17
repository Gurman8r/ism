-- sandbox
project			"sandbox"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/%{cfg.platform}/%{cfg.buildcfg}/"
location		"%{wks.location}/workspace/%{_ACTION}/programs/%{prj.name}"

dependson{ "ism", }

links{ "ism", "assimp", "glew", "glfw", "imgui", }

defines{ "IMGUI_API=ISM_API_IMPORT", }

files{
	"%{wks.location}/programs/%{prj.name}/**.**",
}

postbuildcommands{
	COPYFILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{EXE}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}