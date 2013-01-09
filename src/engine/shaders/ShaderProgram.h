/*
 * ShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <vector>

#include <glm/glm.hpp>

#include "IShaderProgram.h"

#include "Shader.h"

namespace oglre {

namespace shaders {

class ShaderProgram : public IShaderProgram{
public:
	ShaderProgram(std::vector<Shader*> shaders);
	virtual ~ShaderProgram();
	
	virtual void bind();
	virtual GLuint getGLShaderProgramId();
	glm::detail::int32 initialize();
	
	static void unbindAll();

private:
	GLuint programId_;

	std::vector<Shader*> shaders_;
};

}

}

#endif /* SHADERPROGRAM_H_ */
