/**
 * @file CallBackglBegin.hpp
 * @brief This is really annoying to keep typing glBegin, blablabla, glEnd().
 * What about a functional approach ? With call-back functions, it could be funny !
 * 
 * In order to compute this file, you'll need to add the following options :
 *    - \b -lGL
 *    - \b -lGLEW
 *    - \b -std=c++11
 *    - \b 
 *    - \b 
 * @author MTLCRBN
 */
#ifndef CALLBACKGLBEGIN_HP_INCLUDED
#define CALLBACKGLBEGIN_HP_INCLUDED

// Thanks to Jean-Claude Iehl from Universite Lyon 1 for this piece of code.
#ifndef _GK_GL3CORE_H
	#ifdef GK_MACOS
		#include <OpenGL/gl3.h>
		#define NO_GLEW
	#else
		#define GLEW_NO_GLU
		#include "GL/glew.h"
	#endif
#endif
#include <GL/gl.h>
#include <functional>

namespace mtl
{
	namespace gl
	{
		/**
		 * @brief Call glBegin(<b>what</b>), then your code within \b callback, and finaly glEnd(), for you :D.
		 * @param[in] what     The primitive you wanna draw.
		 * @param[in] callback The function you wanna execute between glBegin() and glEnd().
		 * @pre \b what must be a valid GLenum.
		 * @pre A valid gl context must exists !
		 */
		void begin(GLenum what, std::function<void(void)> callback);
	}

}

#endif
