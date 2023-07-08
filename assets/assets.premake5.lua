-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assets
cpp_project("", "assets", "Utility", "%{_BUILD}")

files{ "%{_ASSETS}/**.**", }

prebuildcommands{

	-- copy manifest
	"{MKDIR} %{_BUILD}",
	"{MKDIR} %{_BUILD}/bin",
	"{COPYFILE} %{_PLATFORM}/%{_TARGET_OS}/bin.manifest %{_BUILD}/bin",

	-- copy assets
	"{COPYDIR} %{_ASSETS}/config %{_BUILD}/config",
	"{COPYDIR} %{_ASSETS}/library %{_BUILD}/library",
	"{COPYDIR} %{_ASSETS}/mods %{_BUILD}/mods",
	"{COPYDIR} %{_ASSETS}/resources %{_BUILD}/resources",

}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --