-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- ism
cpp_project("Engine", "ism", "SharedLib", "%{_BUILD_BIN}")

manifest("ism")

links_graphics()

dependson{ "assimp", "freetype2", "glfw", "imgui", "zip", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "zip", "zlibstatic", }

defines{
	"DYNAMIC_BUILD_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{_SLN}ism.premake5.lua",

	"%{_CORE}**.hpp",
	"%{_CORE}**.cpp",
	"%{_CORE}**.inl",
	
	"%{_DRIVERS}register_driver_types.hpp",
	"%{_DRIVERS}register_driver_types.cpp",
	"%{_DRIVERS}%{_TARGET_OS}/**.hpp",
	"%{_DRIVERS}%{_TARGET_OS}/**.cpp",
	"%{_DRIVERS}".._OPTIONS["gfxapi"].."/**.hpp",
	"%{_DRIVERS}".._OPTIONS["gfxapi"].."/**.cpp",
	
	"%{_EDITOR}**.hpp",
	"%{_EDITOR}**.cpp",
	
	"%{_MAIN}**.hpp",
	"%{_MAIN}**.cpp",

	"%{_PLATFORM}%{_TARGET_OS}/**.hpp",
	"%{_PLATFORM}%{_TARGET_OS}/**.cpp",
	
	"%{_SCENE}**.hpp",
	"%{_SCENE}**.cpp",
	
	"%{_SERVERS}**.hpp",
	"%{_SERVERS}**.cpp",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --