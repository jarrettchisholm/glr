/*
 * Shader.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <GL/glew.h>

#include <boost/log/trivial.hpp>

#include "Shader.h"

namespace oglre {

namespace shaders {

Shader::Shader(std::string sourceCode, Type type) : sourceCode_(sourceCode), type_(type) {
	shaderId_ = -1;
}

Shader::~Shader() {
}

IShader::Type Shader::parseType(std::string type) {
	BOOST_LOG_TRIVIAL(debug) << "parsing shader type: " << type;
	if (type.compare("fragment") == 0)
		return IShader::TYPE_FRAGMENT;
	else if (type.compare("vertex") == 0)
		return IShader::TYPE_VERTEX;
	else if (type.compare("geometry") == 0)
		return IShader::TYPE_GEOMETRY;
	else if (type.compare("tessellation") == 0)
		return IShader::TYPE_TESSELLATION;
		
	return IShader::TYPE_NONE;
}

glm::detail::int32 Shader::initialize() {
	BOOST_LOG_TRIVIAL(debug) << "Initializing shader.";
	
	if (shaderId_ < 0) {
		BOOST_LOG_TRIVIAL(error) << "Could not load shader - shader already has an OpenGL id assigned to it.";
		return -1;
	}
	
	
	GLenum shaderType;
	
	switch (type_) {
		case TYPE_NONE:
			BOOST_LOG_TRIVIAL(warning) << "Could not load shader - shader type is not known.";
			break;
			
		case TYPE_VERTEX:
			shaderType = GL_VERTEX_SHADER;
			break;
			
		case TYPE_FRAGMENT:
			shaderType = GL_FRAGMENT_SHADER;
			break;
			
		case TYPE_GEOMETRY:
			shaderType =  GL_GEOMETRY_SHADER;
			break;
			
		case TYPE_TESSELLATION:
			BOOST_LOG_TRIVIAL(warning) << "Could not load shader - tessellation shaders are not yet implemented.";
			break;
	}
	
	
	shaderId_ = glCreateShader(shaderType);
	
	const char* source = sourceCode_.c_str();
	
	BOOST_LOG_TRIVIAL(debug) << "source: " << source;
	
	glShaderSourceARB(shaderId_, 1, &source, nullptr);
	//glShaderSourceARB(fragmentShaderObject, 1, &FragmentShaderSource, &flength);
	
	glCompileShader(shaderId_);
	
	GLint compiled;

	glGetProgramiv(shaderId_, GL_COMPILE_STATUS, &compiled);
	
	if (!compiled) {
		GLint blen = 0;	
		GLsizei slen = 0;
		
		glGetShaderiv(shaderId_, GL_INFO_LOG_LENGTH , &blen);
		
		BOOST_LOG_TRIVIAL(error) << "Could not initialize shader.";     
		
		if (blen > 1) {
			GLchar* compiler_log = (GLchar*)malloc(blen);
			
			glGetInfoLogARB(shaderId_, blen, &slen, compiler_log);
			BOOST_LOG_TRIVIAL(error) << "compiler_log: " << compiler_log;
			free (compiler_log);
		}
		
		return -1;
	}
	
	
	BOOST_LOG_TRIVIAL(debug) << "Done initializing shader.";
	return 0;
}

IShader::Type Shader::getType() {
	return type_;
}

GLuint Shader::getGLShaderId() {
	return shaderId_;
}
	
void Shader::bind() {
	
}

}

}
