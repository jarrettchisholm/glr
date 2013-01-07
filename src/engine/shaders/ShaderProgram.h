/*
 * ShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "IShaderProgram.h"

namespace oglre {

namespace shaders {

class ShaderProgram : public IShaderProgram{
public:
	ShaderProgram();
	virtual ~ShaderProgram();
	
	virtual void bind();
	glm::detail::int32 initialize();
	
	static void unbindAll();

private:
	GLuint textureId_;

	void loadShaderProgram();
};

}

}

#endif /* SHADERPROGRAM_H_ */
