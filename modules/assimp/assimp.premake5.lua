-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assimp
cpp_project("Modules", "assimp", "Utility", "%{_BUILD_BIN}")

module("assimp", 0, 0, 0, "alpha", "official")

manifest("assimp")

includedirs{ "%{_THIRDPARTY}/assimp/include", }

postbuildcommands{
	"{COPYFILE} %{_VENDOR}/assimp.dll %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/assimp.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/IrrXML.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/zlibstatic.lib %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --