-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assets
cpp_project_common("", "assets", "Utility", "%{_BUILD_BIN}")

files{ "%{_ASSETS}**.**", }

prebuildcommands{
	-- copy manifest
	"{MKDIR} %{_BUILD}",
	"{MKDIR} %{_BUILD_BIN}",
	"{COPYFILE} %{_PLATFORM}windows/bin.manifest %{_BUILD_BIN}",

	-- copy assets
	"{COPYDIR} %{_ASSETS}config/ %{_BUILD_CONFIG}",
	"{COPYDIR} %{_ASSETS}data/ %{_BUILD_DATA}",
	"{COPYDIR} %{_ASSETS}defaultconfigs/ %{_BUILD_DEFAULTCONFIGS}",
	"{COPYDIR} %{_ASSETS}mods/ %{_BUILD_MODS}",
	"{COPYDIR} %{_ASSETS}profiles/ %{_BUILD_PROFILES}",
	"{COPYDIR} %{_ASSETS}resources/ %{_BUILD_RESOURCES}",
	"{COPYDIR} %{_ASSETS}saves/ %{_BUILD_SAVES}",
	"{COPYDIR} %{_ASSETS}user/ %{_BUILD_USER}",

	-- guarantee file structure
	"{MKDIR} %{_BUILD_DATA}",
	"{MKDIR} %{_BUILD_CACHE}",
	"{MKDIR} %{_BUILD_CONFIG}",
	"{MKDIR} %{_BUILD_DEFAULTCONFIGS}",
	"{MKDIR} %{_BUILD_DOWNLOADS}",
	"{MKDIR} %{_BUILD_MODS}",
	"{MKDIR} %{_BUILD_PROFILES}",
	"{MKDIR} %{_BUILD_RESOURCES}",
	"{MKDIR} %{_BUILD_SAVES}",
	"{MKDIR} %{_BUILD_USER}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --