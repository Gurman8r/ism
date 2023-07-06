-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

include "./misc/premake5_solution_items.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

newoption{
	trigger		= "gfxapi",
	value		= "api",
	description	= "graphics api",
	default		= "opengl",
	category	= "Build Options",
	allowed		= { { "opengl", "OpenGL" }, { "vulkan", "Vulkan" }, { "directx", "DirectX" }, }
}

newoption{
	trigger		= "glapi",
	value		= "api",
	description	= "opengl loader",
	default		= "glew",
	category	= "Build Options",
	allowed		= { { "glew", "GLEW" }, { "glad", "GLAD" }, }
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- configurations
function load_configurations(_configurations)
	configurations(_configurations)
	filter{ "configurations:Debug" } symbols "On" optimize "Off"
	filter{ "configurations:Release" } symbols "Off" optimize "Speed"
	filter{}
end

-- platforms
function load_platforms(_platforms)
	platforms(_platforms)
	filter{ "platforms:*32", "platforms:*86" } architecture "x86"
	filter{ "platforms:*64" } architecture "x86_64"
	filter{}
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- workspace paths
_SLN		= "%{wks.location}"
_APP		= "%{_SLN}/apps"
_ASSETS		= "%{_SLN}/assets"
_CORE		= "%{_SLN}/core"
_DRIVERS	= "%{_SLN}/drivers"
_EDITOR		= "%{_SLN}/editor"
_EXT		= "%{_SLN}/extensions"
_MAIN		= "%{_SLN}/main"
_MODULES	= "%{_SLN}/modules"
_PLATFORM	= "%{_SLN}/platform"
_PROJECT	= "%{_SLN}/.ism/workspace/%{_ACTION}/%{prj.name}"
_SCENE		= "%{_SLN}/scene"
_SERVERS	= "%{_SLN}/servers"
_THIRDPARTY	= "%{_SLN}/thirdparty"
_TEMPORARY	= "%{_SLN}/.ism/temporary/%{_ACTION}/%{_TARGET_OS}"
_VENDOR		= "%{_SLN}/misc/%{_TARGET_OS}/vendor/%{cfg.platform}/%{cfg.buildcfg}"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- _build paths
_BUILD				= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}"
_BUILD_BIN			= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/bin"
_BUILD_CACHE		= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/cache"
_BUILD_CONFIG		= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/config"
_BUILD_DATA			= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/data"
_BUILD_DEFAULTS		= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/defaultconfigs"
_BUILD_DOWNLOADS	= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/downloads"
_BUILD_MODS			= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/mods"
_BUILD_PROFILES		= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/profiles"
_BUILD_RESOURCES	= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/resources"
_BUILD_SAVES		= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/saves"
_BUILD_USER			= "%{_SLN}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/user"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- cpp project
function cpp_project(_group, _project, _kind, _targetdir)
	group			(_group)
	project			(_project)
	kind			(_kind)
	language		("C++")
	cppdialect		("C++17")
	systemversion	("latest")
	staticruntime	("Off")
	rtti			("On")
	targetname		("%{prj.name}")
	targetdir		(_targetdir)
	objdir			("%{_TEMPORARY}")
	location		("%{_PROJECT}")
end

-- csharp project
function csharp_project(_group, _project, _kind, _targetdir)
	group		(_group)
	project		(_project)
	kind		(_kind)
	language	("C#")
	framework	("4.0")
	targetname	("%{prj.name}")
	targetdir	(_targetdir)
	objdir		("%{_TEMPORARY}")
	location	("%{_PROJECT}")
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- link win32
function links_win32()
	filter{ "language:C++", "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" } defines{ "_CRT_SECURE_NO_WARNINGS" } undefines{ "NDEBUG" }
	filter{ "language:C++", "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
	filter{ "language:C++", "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
	filter{}
end

-- link graphics
function links_graphics()
	filter{ "language:C++", "options:gfxapi=opengl" } links{ "opengl32" } defines{ "OPENGL_ENABLED=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glad" } dependson{ "glad" } links{ "glad" } defines{ "OPENGL_LOADER_GLAD=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew" } dependson{ "glew" } defines{ "OPENGL_LOADER_GLEW=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew", "configurations:Debug" } links{ "glew32d" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew", "configurations:Release" } links{ "glew32" }
	filter{ "language:C++", "options:gfxapi=vulkan" } -- NYI --
	filter{ "language:C++", "options:gfxapi=directx" } -- NYI --
	filter{}
end

-- link assimp
function links_assimp()
	dependson{ "assimp", }
	links{ "assimp.lib", "IrrXML", "zlibstatic", }
end

-- link gui
function links_gui()
	dependson{ "freetype2", "glfw", "imgui", }
	links{ "freetype", "glfw", "imgui", }
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- version
function version(_name, _major, _minor, _patch, _status, _build)
	defines{
		"VERSION_NAME=\"".._name.."\"",
		"VERSION_MAJOR="..tostring(_major).."",
		"VERSION_MINOR="..tostring(_minor).."",
		"VERSION_PATCH="..tostring(_patch).."",
		"VERSION_STATUS=\"".._status.."\"",
		"VERSION_BUILD=\"".._build.."\"",
	}
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- module
function module(_name, _major, _minor, _patch, _status, _build)
	version(_name, _major, _minor, _patch, _status, _build)
	dependson{ "assets", }
	files{
		"%{_MODULES}/".._name.."/".._name..".premake5.lua",
		"%{_MODULES}/".._name.."/**.h",
		"%{_MODULES}/".._name.."/**.c",
		"%{_MODULES}/".._name.."/**.hpp",
		"%{_MODULES}/".._name.."/**.cpp",
		"%{_MODULES}/".._name.."/**.inl",
		"%{_MODULES}/".._name.."/**.pch",
	}
end

-- extension
function extension(_name, _major, _minor, _patch, _status, _build)
	version(_name, _major, _minor, _patch, _status, _build)
	dependson{ "ism", }
	links{ "ism", "zip", }
	links_assimp()
	links_gui()
	defines{
		"ISM_EXT_API=ISM_API_EXPORT",
	}
	files{
		"%{_EXT}/".._name.."/".._name..".premake5.lua",
		"%{_EXT}/".._name.."/".._name..".ini",
		"%{_EXT}/".._name.."/**.h",
		"%{_EXT}/".._name.."/**.c",
		"%{_EXT}/".._name.."/**.hpp",
		"%{_EXT}/".._name.."/**.cpp",
		"%{_EXT}/".._name.."/**.inl",
		"%{_EXT}/".._name.."/**.pch",
	}
	prebuildcommands{
		"{TOUCH} %{_EXT}/".._name.."/".._name..".ini",
	}
	postbuildcommands{
		"{COPYFILE} %{_EXT}/".._name.."/".._name..".ini %{_BUILD_CONFIG}",
	}
end

-- application
function application(_name, _major, _minor, _patch, _status, _build)
	version(_name, _major, _minor, _patch, _status, _build)
	dependson{ "ism", "lua", "mono", }
	links{ "ism", "mono-2.0-sgen", "zip", }
	links_graphics()
	links_assimp()
	links_gui()
	defines{
		"MAIN_ENABLED=1",
		"IMGUI_API=ISM_API_IMPORT",
	}
	files{
		"%{_PLATFORM}/%{_TARGET_OS}/%{_TARGET_OS}_main.cpp",
		"%{_APP}/".._name.."/".._name..".premake5.lua",
		"%{_APP}/".._name.."/".._name..".rc",
		"%{_APP}/".._name.."/".._name..".ico",
		"%{_APP}/".._name.."/".._name..".ini",
		"%{_APP}/".._name.."/**.h",
		"%{_APP}/".._name.."/**.c",
		"%{_APP}/".._name.."/**.hpp",
		"%{_APP}/".._name.."/**.cpp",
		"%{_APP}/".._name.."/**.inl",
		"%{_APP}/".._name.."/**.pch",
	}
	prebuildcommands{
		"{TOUCH} %{_APP}/".._name.."/".._name..".ini",
	}
	postbuildcommands{
		"{COPYFILE} %{_APP}/".._name.."/".._name..".ini %{_BUILD_CONFIG}",
	}
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- binary manifest

_MANIFEST={}

function manifest(...)
	local args={...}
	for _, v in ipairs(args) do
		table.insert(_MANIFEST, v)
	end
end

function generate_manifest(path)
	local text="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
	text=text.."<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\n"
	text=text.."<assemblyIdentity type=\"win32\" name=\"bin\" version=\"1.0.0.0\"/>\n"
	for _, v in ipairs(_MANIFEST) do
		text=text..string.format("\t<file name=\"%s.dll\"/>\n", v)
	end
	text=text.."</assembly>"
	io.writefile(path, text);
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --