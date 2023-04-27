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
	trigger		= "gl_loader",
	value		= "loader",
	description	= "opengl loader",
	default		= "glew",
	category	= "Build Options",
	allowed		= { { "glew", "GLEW" }, { "glad", "GLAD" }, }
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter{ "system:not windows" }
	LIB="" DLL = "" EXE = ""
filter{ "system:windows" }
	LIB=".lib" DLL = ".dll" EXE = ".exe"
filter{}


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

function srcdirs(...)
	local arg = { ... }
	for i, v in ipairs(arg) do
		files{
			"" .. v .. "/**.hpp",
			"" .. v .. "/**.cpp",
			"" .. v .. "/**.h",
			"" .. v .. "/**.c",
			"" .. v .. "/**.inl",
			"" .. v .. "/**.ini",
			"" .. v .. "/**.cfg",
			"" .. v .. "/**.lua",
			"" .. v .. "/**.cs",
		}
	end
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --