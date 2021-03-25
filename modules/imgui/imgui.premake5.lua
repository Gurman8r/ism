-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- imgui
project			"imgui"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"
targetdir		"%{wks.location}/bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{wks.location}/bin-obj/%{cfg.platform}/%{cfg.buildcfg}/"
location		"%{wks.location}/workspace/%{_ACTION}/modules/%{prj.name}"

dependson{ "glfw", }

links{ "glfw", }

defines{
	"IMGUI_API=__declspec(dllexport)",
	"IMGUI_IMPL_OPENGL_LOADER_GLEW",
}

includedirs{
	"%{wks.location}/thirdparty",
	"%{wks.location}/thirdparty/glfw/glfw/include",
	"%{wks.location}/thirdparty/imgui/imgui",
	"%{wks.location}/thirdparty/imgui/imgui-node-editor",
}

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
	
	"%{wks.location}/thirdparty/imgui/*.h",
	"%{wks.location}/thirdparty/imgui/*.cpp",

	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_glfw.h",
	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_glfw.cpp",
	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_opengl3.h",
	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_opengl3.cpp",
	
	"%{wks.location}/thirdparty/imgui_club/imgui_memory_editor/imgui_memory_editor.h",
	"%{wks.location}/thirdparty/ImGuizmo/ImGuizmo.h",
	"%{wks.location}/thirdparty/ImGuizmo/ImGuizmo.cpp",
	--"%{wks.location}/thirdparty/ImGuiColorTextEdit/*.h",
	--"%{wks.location}/thirdparty/ImGuiColorTextEdit/*.cpp",
	--"%{wks.location}/thirdparty/imgui-node-editor/*.h",
	--"%{wks.location}/thirdparty/imgui-node-editor/*.cpp",
	--"%{wks.location}/thirdparty/implot/**.h",
	--"%{wks.location}/thirdparty/implot/**.cpp",
}

postbuildcommands{
	COPYFILE("%{wks.location}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/%{prj.targetname}%{DLL}", "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}/"),
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --