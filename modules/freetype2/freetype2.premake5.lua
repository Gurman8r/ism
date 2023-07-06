-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- freetype2
cpp_project("Modules", "freetype2", "Utility", "%{_BUILD_BIN}")

module("freetype2", 0, 0, 0, "alpha", "official")

postbuildcommands{
	"{COPYFILE} %{_VENDOR}/freetype.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/freetype.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --