-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glfw
project			"glfw"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
debugdir		"%{wks.location}/build/bin/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
targetdir		"%{wks.location}/temps/lib/%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
objdir			"%{wks.location}/temps/obj/%{_TARGET_OS}/"
location		"%{wks.location}/workspace/%{_ACTION}/modules/%{prj.name}/"

defines{ "_GLFW_BUILD_DLL", }

undefines{ "__NULL_IMPORT_DESCRIPTOR", }

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
	
	"%{wks.location}/thirdparty/glfw/include/glfw/**.h",
	"%{wks.location}/thirdparty/glfw/src/context.c", 
	"%{wks.location}/thirdparty/glfw/src/egl_context.c",
	"%{wks.location}/thirdparty/glfw/src/egl_context.h",
	"%{wks.location}/thirdparty/glfw/src/init.c", 
	"%{wks.location}/thirdparty/glfw/src/input.c", 
	"%{wks.location}/thirdparty/glfw/src/internal.h", 
	"%{wks.location}/thirdparty/glfw/src/mappings.h", 
	"%{wks.location}/thirdparty/glfw/src/monitor.c", 
	"%{wks.location}/thirdparty/glfw/src/osmesa_context.c",
	"%{wks.location}/thirdparty/glfw/src/osmesa_context.h",
	"%{wks.location}/thirdparty/glfw/src/vulkan.c",
	"%{wks.location}/thirdparty/glfw/src/wgl_context.c",
	"%{wks.location}/thirdparty/glfw/src/wgl_context.h",
	"%{wks.location}/thirdparty/glfw/src/window.c",
}

postbuildcommands{
	"{COPYFILE} %{cfg.targetdir}/%{prj.targetname}%{DLL} %{cfg.debugdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter{ "system:windows" }
	defines{ "_GLFW_WIN32=1", }
	files{ "%{wks.location}/thirdparty/glfw/src/win32_**.c", }
filter{}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --