-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
cpp_project("Extensions", "mono", "SharedLib", "%{_BUILD_BIN}")

prepare_extension("mono")

manifest("mono-2.0-sgen")

dependson{ "ism-CSharp", }

links{ "mono-2.0-sgen", }

includedirs{ "%{_THIRDPARTY}mono/include/", }

prebuildcommands{
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen%{DLL} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- ism-CSharp
csharp_project("Engine", "ism-CSharp", "SharedLib", "%{_BUILD_BIN}")

files{ "%{_EXT}mono/**.cs", }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --