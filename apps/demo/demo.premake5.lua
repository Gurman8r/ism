-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo
cpp_project("Demo", "demo", "ConsoleApp", "%{_BUILD}")

application("demo", 0, 0, 0, "alpha", "official")

dependson{ "demo-csharp" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-csharp
csharp_project("Demo", "demo-csharp", "SharedLib", "%{_BUILD}/bin")

dependson{ "ism-csharp" }

links{ "ism-csharp" }

files{ "%{_APP}/demo/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --