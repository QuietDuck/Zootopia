#pragma once

/*
RfStateGL
*/

#ifndef __RF_STATE_GL_H__
#define __RF_STATE_GL_H__

#include <GL/glew.h>

namespace RfStateGL {

    GLenum errorCheckGL(char* fileName, char* funcName, int lineNumb);
}


#ifdef _DEBUG
#define RF_GL_CHECK_ERROR() \
	RfStateGL::errorCheckGL(__xfilename__, __xfuncname__, __xlinenumber__)

#else
#define RF_GL_CHECK_ERROR() 

#endif

#endif