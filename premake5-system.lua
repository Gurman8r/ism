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
_CORE		= "%{_SLN}core/"
_DRIVERS	= "%{_SLN}drivers/"
_EDITOR		= "%{_SLN}editor/"
_MAIN		= "%{_SLN}main/"
_MODULES	= "%{_SLN}modules/"
_PLATFORM	= "%{_SLN}platform/"
_PROGRAMS	= "%{_SLN}programs/"
_SCENE		= "%{_SLN}scene/"
_SERVERS	= "%{_SLN}servers/"

-- project paths
_ASSETS		= "%{_SLN}assets/"
_TEMPS		= "%{_SLN}tmp/%{_ACTION}/%{_TARGET_OS}/"
_THIRDPARTY	= "%{_SLN}thirdparty/"
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

-- C++ project common
function cpp_project_common(_group, _project, _kind, _targetdir)
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
	objdir			("%{_TEMPS}")
	location		("%{_PROJECT}")
end

-- C# project common
function csharp_project_common(_group, _project, _kind, _targetdir)
	group		(_group)
	project		(_project)
	kind		(_kind)
	language	("C#")
	framework	("4.0")
	targetname	("%{prj.name}")
	targetdir	(_targetdir)
	objdir		("%{_TEMPS}")
	location	("%{_PROJECT}")
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- graphics links
function links_graphics()
	filter{ "language:C++", "options:gfxapi=opengl" } links{ "opengl32" } defines{ "OPENGL_ENABLED=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glad" } dependson{ "glad" } links{ "glad" } defines{ "OPENGL_LOADER_GLAD=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew" } dependson{ "glew" } defines{ "OPENGL_LOADER_GLEW=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew", "configurations:Debug" } links{ "glew32d" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew", "configurations:Release" } links{ "glew32" }
	filter{ "language:C++", "options:gfxapi=vulkan" } -- WIP --
	filter{ "language:C++", "options:gfxapi=directx" } -- WIP --
	filter{}
end

-- windows links
function links_win32()
	filter{ "language:C++", "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" } defines{ "_CRT_SECURE_NO_WARNINGS" } undefines{ "NDEBUG" }
	filter{ "language:C++", "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
	filter{ "language:C++", "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
	filter{}
end

-- mark executable
function mark_as_executable()
	dependson{ "ism", "lua", "mono", }
	links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "ism", "mono-2.0-sgen", "zip", "zlibstatic", }
	files{ "%{_PLATFORM}%{_TARGET_OS}/%{_TARGET_OS}_main.cpp", }
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

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