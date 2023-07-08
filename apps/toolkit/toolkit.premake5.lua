-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- toolkit
cpp_project("Editor", "toolkit", "ConsoleApp", "%{_BUILD}")

application("toolkit", 0, 0, 0, "alpha", "official")

dependson{ "toolkit-csharp" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- toolkit-csharp
csharp_project("Editor", "toolkit-csharp", "SharedLib", "%{_BUILD}/bin")

dependson{ "ism-csharp" }

links{ "ism-csharp" }

files{ "%{_APP}/toolkit/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --