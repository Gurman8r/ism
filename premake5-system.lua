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

-- source paths
_SLN		= "%{wks.location}/"
_APP		= "%{_SLN}app/"
_CORE		= "%{_SLN}core/"
_DRIVERS	= "%{_SLN}drivers/"
_EDITOR		= "%{_SLN}editor/"
_EXT		= "%{_SLN}ext/"
_MAIN		= "%{_SLN}main/"
_MODULES	= "%{_SLN}modules/"
_PLATFORM	= "%{_SLN}platform/"
_SCENE		= "%{_SLN}scene/"
_SERVERS	= "%{_SLN}servers/"

-- project paths
_ASSETS		= "%{_SLN}assets/"
_THIRDPARTY	= "%{_SLN}thirdparty/"
_TMP		= "%{_SLN}tmp/%{_ACTION}/%{_TARGET_OS}/"
_VENDOR		= "%{_SLN}misc/%{_TARGET_OS}/vendor/%{cfg.platform}/%{cfg.buildcfg}/"
_PROJECT	= "%{_SLN}workspace/%{_ACTION}/%{prj.name}/"

-- build paths
_BUILD					= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
_BUILD_BIN				= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/bin/"
_BUILD_CACHE			= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/cache/"
_BUILD_CONFIG			= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/config/"
_BUILD_DATA				= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/data/"
_BUILD_DEFAULTCONFIGS	= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/defaultconfigs/"
_BUILD_DOWNLOADS		= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/downloads/"
_BUILD_MODS				= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/mods/"
_BUILD_PROFILES			= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/profiles/"
_BUILD_RESOURCES		= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/resources/"
_BUILD_SAVES			= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/saves/"
_BUILD_USER				= "%{_SLN}build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/user/"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- platform specific extensions
LIB=".a" DLL=".so" EXE=""
if _TARGET_OS=="windows" then
LIB=".lib" DLL=".dll" EXE=".exe"
end

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
	objdir			("%{_TMP}")
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
	objdir		("%{_TMP}")
	location	("%{_PROJECT}")
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- links win32
function links_win32()
	filter{ "language:C++", "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" } defines{ "_CRT_SECURE_NO_WARNINGS" } undefines{ "NDEBUG" }
	filter{ "language:C++", "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
	filter{ "language:C++", "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
	filter{}
end

-- links graphics
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

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- prepare extension
function prepare_extension(_name)
	dependson{ "ism" }
	defines{ "ISM_MOD_API=ISM_API_EXPORT", }
	links{ "ism", "zip", "glfw", "imgui", "freetype", "assimp%{LIB}", "IrrXML", "zlibstatic", }
	files{
		"%{_EXT}".._name.."/".._name..".premake5.lua",
		"%{_EXT}".._name.."/".._name..".ini",
		"%{_EXT}".._name.."/**.hpp",
		"%{_EXT}".._name.."/**.cpp",
	}
	postbuildcommands{
		"{COPYFILE} %{_EXT}".._name.."/".._name..".ini %{_BUILD_CONFIG}",
	}
end

-- prepare application
function prepare_application(_name)
	links_graphics()
	dependson{ "ism", "lua", "mono", }
	defines{ "MAIN_ENABLED=1", "IMGUI_API=ISM_API_IMPORT", }
	links{ "ism", "mono-2.0-sgen", "zip", "glfw", "imgui", "freetype", "assimp%{LIB}", "IrrXML", "zlibstatic", }
	files{
		"%{_PLATFORM}%{_TARGET_OS}/%{_TARGET_OS}_main.cpp",
		"%{_APP}".._name.."/".._name..".premake5.lua",
		"%{_APP}".._name.."/".._name..".ico",
		"%{_APP}".._name.."/".._name..".rc",
	}
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- binary manifest
_MANIFEST={}

function manifest(...)
	local args={...}
	for i, v in ipairs(args) do
		table.insert(_MANIFEST, v)
	end
end

function generate_manifest(path)
	local text="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
	text=text.."<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\n"
	text=text.."<assemblyIdentity type=\"win32\" name=\"bin\" version=\"1.0.0.0\"/>\n"
	for i, v in ipairs(_MANIFEST) do
		text=text..string.format("\t<file name=\"%s%s\"/>\n", v, DLL)
	end
	text=text.."</assembly>"
	io.writefile(path, text);
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --