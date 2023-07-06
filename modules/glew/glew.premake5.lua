-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glew
cpp_project("Modules", "glew", "Utility", "%{_BUILD_BIN}")

module("glew", 0, 0, 0, "alpha", "official")

manifest("glew32", "glew32d")

files{ "%{_MODULES}/glew/glew.premake5.lua", }

filter{ "configurations:Debug" } postbuildcommands{ "{COPYFILE} %{_VENDOR}/glew32d.dll %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}/glew32d.lib %{cfg.targetdir}", }
filter{ "configurations:Release" } postbuildcommands{ "{COPYFILE} %{_VENDOR}/glew32.dll %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}/glew32.lib %{cfg.targetdir}", }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --