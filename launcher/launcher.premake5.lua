-- launcher
project			"ism-launcher"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
targetdir		"%{_BUILD}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

dependson{ "ism", "lua", "mono", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "ism", "zip", "zlibstatic", }

links_graphics()

defines{
	"MAIN_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/assets/**.**",
	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
}

srcdirs(
	"%{wks.location}/launcher/"
)

filter{ "system:windows" }
	files{
		"%{wks.location}/platform/windows/ism.rc",
		"%{wks.location}/platform/windows/ism.ico",
	}
	postbuildcommands{
		"{COPYFILE} %{wks.location}/platform/windows/binaries.manifest %{_BUILD_BIN}",
		"{COPYFILE} %{wks.location}/platform/windows/ism.ico %{_BUILD_RES}",
		"{COPYFILE} %{wks.location}/assets/ism.png %{_BUILD_RES}",
	}
filter{}