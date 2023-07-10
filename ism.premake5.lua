-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- ism
cpp_project("Engine", "ism", "SharedLib", "%{_BUILD}/bin")

version("ism", 0, 0, 0, "alpha", "official")

manifest("ism")

dependson{ "zip", }

links{ "zip", }
links_graphics()
links_assimp()
links_gui()

defines{
	"TOOLS_ENABLED=1",
	"ISM_DYNAMIC=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{_SLN}/ism.premake5.lua",

	"%{_CORE}/**.hpp",
	"%{_CORE}/**.cpp",
	"%{_CORE}/**.inl",
	
	"%{_DRIVERS}/register_driver_types.hpp",
	"%{_DRIVERS}/register_driver_types.cpp",
	"%{_DRIVERS}/%{_TARGET_OS}/**.hpp",
	"%{_DRIVERS}/%{_TARGET_OS}/**.cpp",
	"%{_DRIVERS}/".._OPTIONS["gfxapi"].."/**.hpp",
	"%{_DRIVERS}/".._OPTIONS["gfxapi"].."/**.cpp",
	
	"%{_EDITOR}/**.hpp",
	"%{_EDITOR}/**.cpp",
	
	"%{_MAIN}/**.hpp",
	"%{_MAIN}/**.cpp",

	"%{_PLATFORM}/%{_TARGET_OS}/**.hpp",
	"%{_PLATFORM}/%{_TARGET_OS}/**.cpp",
	
	"%{_SCENE}/**.hpp",
	"%{_SCENE}/**.cpp",
	
	"%{_SERVERS}/**.hpp",
	"%{_SERVERS}/**.cpp",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --