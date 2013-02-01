/*
 * Shader.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <sstream>

#include <GL/glew.h>

#include <boost/log/trivial.hpp>

#include "Shader.h"

#include "../exceptions/GlException.h"

namespace oglre {

namespace shaders {

Shader::Shader(std::string sourceCode, Type type) : sourceCode_(sourceCode), type_(type) {
	shaderId_ = -1;
	
	initialize();
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

void Shader::initialize() {
	BOOST_LOG_TRIVIAL(debug) << "Initializing shader.";
	
	if (shaderId_ < 0) {
		std::string msg("Could not load shader - shader already has an OpenGL id assigned to it.");
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::GlException(msg);
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
			std::string msg("Could not load shader - tessellation shaders are not yet implemented.");
			BOOST_LOG_TRIVIAL(warning) << msg;
			throw exception::GlException(msg);
	}
	
	
	shaderId_ = glCreateShader(shaderType);
	
	const char* source = sourceCode_.c_str();
	
	BOOST_LOG_TRIVIAL(debug) << "source: " << source;
	
	glShaderSourceARB(shaderId_, 1, &source, nullptr);
	//glShaderSourceARB(fragmentShaderObject, 1, &FragmentShaderSource, &flength);
	
	glCompileShader(shaderId_);
	
	GLint compiled;

	glGetProgramiv(shaderId_, GL_COMPILE_STATUS, &compiled);
	
	// Handle any errors
	if (compiled = GL_FALSE) {
		std::stringstream msg;
		msg << "Could not initialize shader.";
		
		GLint infoLogLength;
	    glGetShaderiv(shaderId_, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	    GLchar* strInfoLog = new GLchar[infoLogLength + 1];
	    glGetShaderInfoLog(shaderId_, infoLogLength, NULL, strInfoLog);
	    
	    msg << "\ncompiler_log: " << strInfoLog;
	
	    delete[] strInfoLog;
		
		BOOST_LOG_TRIVIAL(error) << msg.str();
		
		throw exception::GlException(msg.str());
	}
	
	
	BOOST_LOG_TRIVIAL(debug) << "Done initializing shader.";
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
