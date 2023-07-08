-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
cpp_project("Extensions", "mono", "SharedLib", "%{_BUILD}/bin")

extension("mono", 0, 0, 0, "alpha", "official")

manifest("mono-2.0-sgen")

dependson{ "ism-csharp" }

links{ "mono-2.0-sgen" }

includedirs{ "%{_THIRDPARTY}/mono/include" }

prebuildcommands{
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.dll %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.lib %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- ism-csharp
csharp_project("Engine", "ism-csharp", "SharedLib", "%{_BUILD}/bin")

files{ "%{_EXT}/mono/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --