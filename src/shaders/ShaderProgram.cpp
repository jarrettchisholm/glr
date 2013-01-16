/*
 * ShaderProgram.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */
#include <boost/log/trivial.hpp>

#include "ShaderProgram.h"

namespace oglre {

namespace shaders {

ShaderProgram::ShaderProgram(std::vector<Shader*> shaders) : shaders_(shaders) {
	programId_ = -1;
}

ShaderProgram::~ShaderProgram() {
}

glm::detail::int32 ShaderProgram::initialize() {
	BOOST_LOG_TRIVIAL(debug) << "Initializing shader program.";
	
	if (programId_ < 0) {
		BOOST_LOG_TRIVIAL(error) << "Could not load shader program - shader program already has an OpenGL id assigned to it.";
		return -1;
	}
	
	programId_ = glCreateProgram();
	
	for (int i=0; i < shaders_.size(); i++) {
		glAttachShader(programId_, shaders_[i]->getGLShaderId());
	}
	
	glBindAttribLocation(programId_, 0, "in_Position");
	glBindAttribLocation(programId_, 1, "in_Texture");
	glBindAttribLocation(programId_, 2, "in_Color");
	
	glLinkProgram(programId_); 
	
	GLint linked;
	glGetProgramiv(programId_, GL_LINK_STATUS, &linked);
	
	if (!linked) {
		BOOST_LOG_TRIVIAL(error) << "Could not initialize shader program.";
		
		GLchar errorLog[1024] = {0};
	    glGetProgramInfoLog(programId_, 1024, nullptr, errorLog);
	    
	    BOOST_LOG_TRIVIAL(error) << errorLog;
		
		return -1;
	}  
	
	
	BOOST_LOG_TRIVIAL(debug) << "Done initializing shader program.";
	return 0;
}

GLuint ShaderProgram::getGLShaderProgramId() {
	return programId_;
}
	
void ShaderProgram::bind() {
	glUseProgram(programId_);
}

void ShaderProgram::unbindAll() {
	glUseProgram(0);
}

}

}
