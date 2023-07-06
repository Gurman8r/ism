-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- zip
cpp_project("Modules", "zip", "StaticLib", "%{_BUILD_BIN}")

module("zip", 0, 0, 0, "alpha", "official")

includedirs{ "%{_THIRDPARTY}/minizip/", "%{_THIRDPARTY}/zlib/", }

files{ "%{_THIRDPARTY}/minizip/**.h", "%{_THIRDPARTY}/minizip/**.c", "%{_THIRDPARTY}/zlib/**.h", "%{_THIRDPARTY}/zlib/**.c", }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --