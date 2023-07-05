-- toolkit
cpp_project("Editor", "toolkit", "ConsoleApp", "%{_BUILD}")
dependson{ "toolkit-CS", }
links_graphics()
main_executable()
files{ "%{_PROGRAMS}toolkit/toolkit.premake5.lua", "%{_PROGRAMS}toolkit/toolkit.ico", "%{_PROGRAMS}toolkit/toolkit.rc", }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- toolkit-CS
csharp_project("Editor", "toolkit-CS", "SharedLib", "%{_BUILD_BIN}")
links{"ism-CS"}
files{"%{_PROGRAMS}toolkit/**.cs"}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --