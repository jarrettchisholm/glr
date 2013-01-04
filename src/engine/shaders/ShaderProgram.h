/*
 * ShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <GL/gl.h>

namespace oglre {

namespace shaders {

class ShaderProgram {
public:
	ShaderProgram();
	virtual ~ShaderProgram();
	
	void bind();

private:
	GLuint textureId_;

	void loadShaderProgram();
};

}

}

#endif /* SHADERPROGRAM_H_ */
