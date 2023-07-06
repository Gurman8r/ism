-- demo
cpp_project("Demo", "demo", "ConsoleApp", "%{_BUILD}")

prepare_application("demo")

dependson{ "demo-CSharp" }

files{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-CSharp
csharp_project("Demo", "demo-CSharp", "SharedLib", "%{_BUILD_BIN}")

dependson{ "ism-CSharp" }

links{ "ism-CSharp" }

files{ "%{_APP}demo/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --