-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
cpp_project_common("Modules", "mono", "SharedLib", "%{_BUILD_BIN}")

manifest("mono-2.0-sgen")

dependson{ "ism", "ism-CS", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "ism", "mono-2.0-sgen", "zip", "zlibstatic", }

defines{ "ISM_MOD_API=ISM_API_EXPORT", }

includedirs{ "%{_THIRDPARTY}mono/include/", }

files{
	"%{_MODULES}mono/mono.premake5.lua",
	"%{_MODULES}mono/mono.ini",
	"%{_MODULES}mono/**.hpp",
	"%{_MODULES}mono/**.cpp",
}

prebuildcommands{
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen%{DLL} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen.pdb %{cfg.targetdir}",
}

postbuildcommands{
	"{COPYFILE} %{_MODULES}mono/mono.ini %{_BUILD_CFG}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- ism-CS
csharp_project_common("Engine", "ism-CS", "SharedLib", "%{_BUILD_BIN}")

files{ "%{_MODULES}mono/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --