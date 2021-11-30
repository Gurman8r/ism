#if OPENGL_ENABLED
#ifndef _ISM_OPENGL_HPP_
#define _ISM_OPENGL_HPP_

#include <core/preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// OpenGL Loader

#ifdef OPENGL_LOADER_CUSTOM
#	if __has_include(OPENGL_LOADER_CUSTOM)
#		include OPENGL_LOADER_CUSTOM
#	endif

#elif defined(OPENGL_LOADER_ES2)
#	include <GLES2/gl2.h>
#	define OPENGL_INIT() (false)

#elif defined(OPENGL_LOADER_ES3)
#	if defined(os_apple) && (TARGET_OS_IOS || TARGET_OS_TV)
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <GLES3/gl3.h>
#	endif
#	define OPENGL_INIT() (false)

#elif defined(OPENGL_LOADER_GLEW)
#	include <GL/glew.h>
#	define OPENGL_INIT() ((glewExperimental = true), (glewInit() == GLEW_OK))

#elif defined(OPENGL_LOADER_GL3W)
#	include <GL/gl3w.h>
#	define OPENGL_INIT() (gl3wInit())

#elif defined(OPENGL_LOADER_GLAD)
#	include <glad/glad.h>
#	define OPENGL_INIT() (gladLoadGL())

#else
#	error "Can't find OpenGL'"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_OPENGL_HPP_
#endif // OPENGL_ENABLED
