-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- ism
cpp_project("Engine", "ism", "SharedLib", "%{_BUILD_BIN}")

version("ism", 0, 0, 0, "alpha", "official")

manifest("ism")

dependson{ "zip", }

links{ "zip", }
links_graphics()
links_assimp()
links_gui()

defines{
	"TOOLS_ENABLED=1",
	"DYNAMIC_BUILD_ENABLED=1",
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

-- assets
cpp_project("", "assets", "Utility", "%{_BUILD_BIN}")

files{ "%{_ASSETS}/**.**", }

prebuildcommands{

	-- copy bin manifest
	"{MKDIR} %{_BUILD}",
	"{MKDIR} %{_BUILD_BIN}",
	"{COPYFILE} %{_PLATFORM}/windows/bin.manifest %{_BUILD_BIN}",

	-- copy assets
	"{COPYDIR} %{_ASSETS}/config/ %{_BUILD_CONFIG}",
	"{COPYDIR} %{_ASSETS}/data/ %{_BUILD_DATA}",
	"{COPYDIR} %{_ASSETS}/defaultconfigs/ %{_BUILD_DEFAULTS}",
	"{COPYDIR} %{_ASSETS}/mods/ %{_BUILD_MODS}",
	"{COPYDIR} %{_ASSETS}/profiles/ %{_BUILD_PROFILES}",
	"{COPYDIR} %{_ASSETS}/resources/ %{_BUILD_RESOURCES}",
	"{COPYDIR} %{_ASSETS}/saves/ %{_BUILD_SAVES}",
	"{COPYDIR} %{_ASSETS}/user/ %{_BUILD_USER}",

	-- guarantee file structure
	"{MKDIR} %{_BUILD_DATA}",
	"{MKDIR} %{_BUILD_CACHE}",
	"{MKDIR} %{_BUILD_CONFIG}",
	"{MKDIR} %{_BUILD_DEFAULTS}",
	"{MKDIR} %{_BUILD_DOWNLOADS}",
	"{MKDIR} %{_BUILD_MODS}",
	"{MKDIR} %{_BUILD_PROFILES}",
	"{MKDIR} %{_BUILD_RESOURCES}",
	"{MKDIR} %{_BUILD_SAVES}",
	"{MKDIR} %{_BUILD_USER}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --