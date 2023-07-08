-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glfw
cpp_project("Modules", "glfw", "SharedLib", "%{_BUILD}/bin")

links_graphics()

module("glfw", 0, 0, 0, "alpha", "official")

manifest("glfw")

defines{ "_GLFW_BUILD_DLL", }

undefines{ "__NULL_IMPORT_DESCRIPTOR", }

files{
	"%{_THIRDPARTY}/glfw/include/glfw/**.h",
	"%{_THIRDPARTY}/glfw/src/context.c", 
	"%{_THIRDPARTY}/glfw/src/egl_context.c",
	"%{_THIRDPARTY}/glfw/src/egl_context.h",
	"%{_THIRDPARTY}/glfw/src/init.c", 
	"%{_THIRDPARTY}/glfw/src/input.c", 
	"%{_THIRDPARTY}/glfw/src/internal.h", 
	"%{_THIRDPARTY}/glfw/src/mappings.h", 
	"%{_THIRDPARTY}/glfw/src/monitor.c", 
	"%{_THIRDPARTY}/glfw/src/osmesa_context.c",
	"%{_THIRDPARTY}/glfw/src/osmesa_context.h",
	"%{_THIRDPARTY}/glfw/src/vulkan.c",
	"%{_THIRDPARTY}/glfw/src/wgl_context.c",
	"%{_THIRDPARTY}/glfw/src/wgl_context.h",
	"%{_THIRDPARTY}/glfw/src/window.c",
}

filter{ "system:windows" }
	defines{ "_GLFW_WIN32=1", }
	files{ "%{_THIRDPARTY}/glfw/src/win32_**.c", }
filter{}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --