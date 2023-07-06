-- toolkit
cpp_project("Editor", "toolkit", "ConsoleApp", "%{_BUILD}")

prepare_application("toolkit")

dependson{ "toolkit-CSharp" }

files{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- toolkit-CSharp
csharp_project("Editor", "toolkit-CSharp", "SharedLib", "%{_BUILD_BIN}")

dependson{ "ism-CSharp" }

links{ "ism-CSharp" }

files{ "%{_APP}toolkit/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --