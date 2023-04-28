-- launcher
project			"ism-launcher"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
debugdir		"%{_BUILD}"
targetdir		"%{_BUILD}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

debugenvs{ "%{_BUILD}", }

dependson{ "ism", "lua", "mono", }

links{ "assimp%{LIB}", "IrrXML", "zlibstatic", "freetype", "glfw", "imgui", "ism", }

defines{
	"MAIN_ENABLED=true",
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/assets/**.**",
	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
}

srcdirs(
	"%{wks.location}/launcher/"
)

if _TARGET_OS=="windows" then
	files{
		"%{wks.location}/platform/windows/ism.rc",
		"%{wks.location}/platform/windows/ism.ico",
	}
	prebuildcommands{
		"{COPYFILE} %{wks.location}/launcher/bin.manifest %{_BUILD_BIN}",
	}
	postbuildcommands{
		"{COPYFILE} %{wks.location}/platform/windows/ism.ico %{_BUILD_RES}",
	}
end