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

LIB="" DLL = "" EXE = ""
if _TARGET_OS=="windows" then
	LIB=".lib" DLL = ".dll" EXE = ".exe"
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

function srcdirs(...)
	local arg = { ... }
	for i, v in ipairs(arg) do
		files{
			""..v.."/**.hpp",
			""..v.."/**.cpp",
			""..v.."/**.h",
			""..v.."/**.c",
			""..v.."/**.inl",
			""..v.."/**.ini",
			""..v.."/**.cfg",
			""..v.."/**.lua",
			""..v.."/**.cs",
		}
	end
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
		text=text..string.format("\t<file name=\"%s.dll\"/>\n", v)
	end
	text=text.."</assembly>"
	io.writefile(path, text);
end


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --