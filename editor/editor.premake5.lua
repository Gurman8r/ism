-- editor
project			"editor"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/"
location		"%{wks.location}/workspace/%{_ACTION}/programs/%{prj.name}/"

links{ "ism", }

defines{
	"MAIN_ENABLED=true",
	"TOOLS_ENABLED=true",
	"OPENGL_ENABLED=true",
	"OPENGL_LOADER_GLEW=true",
	"ISM_API=ISM_API_IMPORT",
	"ISM_MOD_API=ISM_API_IMPORT",
	"ISM_EDITOR_API=ISM_API_IMPORT",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/assets/**.**",
	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
	"%{wks.location}/editor/editor.premake5.lua",
	"%{wks.location}/editor/**.hpp",
	"%{wks.location}/editor/**.cpp",
	"%{wks.location}/editor/**.h",
	"%{wks.location}/editor/**.c",
	"%{wks.location}/editor/**.inl",
}

postbuildcommands{
	"{COPY} %{cfg.targetdir}/%{prj.targetname}%{EXE} %{cfg.debugdir}",
}