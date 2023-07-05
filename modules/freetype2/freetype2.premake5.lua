-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- freetype2
cpp_project("Modules", "freetype2", "Utility", "%{_BUILD_BIN}")

dependson{ "assets", }

files{
	"%{_MODULES}%{prj.name}/%{prj.name}.premake5.lua",
}

postbuildcommands{
	"{COPYFILE} %{_VENDOR}freetype%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}freetype.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --