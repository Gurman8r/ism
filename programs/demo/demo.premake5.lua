-- demo
cpp_project("Demo", "demo", "ConsoleApp", "%{_BUILD}")
dependson{"demo-CS"}
links_graphics()
main_executable()
files{ "%{_PROGRAMS}demo/demo.premake5.lua", "%{_PROGRAMS}demo/demo.ico", "%{_PROGRAMS}demo/demo.rc", }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-CS
csharp_project("Demo", "demo-CS", "SharedLib", "%{_BUILD_BIN}")
links{"ism-CS"}
files{"%{_PROGRAMS}demo/**.cs"}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --