/*
 * IShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADERPROGRAM_H_
#define ISHADERPROGRAM_H_

#include <GL/glew.h>

#include "IShader.h"

namespace glr {
namespace shaders {
class IShaderProgram {
public:
	virtual ~IShaderProgram()
	{
	}
	;

	virtual void bind() = 0;

	virtual GLuint getGLShaderProgramId() = 0;

	virtual IShader::BindingsMap getBindings() = 0;
};
}
}
#endif /* ISHADERPROGRAM_H_ */
