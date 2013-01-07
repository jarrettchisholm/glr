/*
 * ShaderProgram.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include "ShaderProgram.h"

namespace oglre {

namespace shaders {

ShaderProgram::ShaderProgram() {
	
}

ShaderProgram::~ShaderProgram() {
}

glm::detail::int32 ShaderProgram::initialize() {
	
}
	
void ShaderProgram::bind() {
	
}

void ShaderProgram::unbindAll() {
	glUseProgram(0);
}

}

}
