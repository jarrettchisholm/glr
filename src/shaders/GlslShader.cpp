/*
 * GlslShader.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <sstream>

#include <GL/glew.h>

#include <boost/log/trivial.hpp>

#include "GlslShader.h"

#include "../exceptions/GlException.h"

namespace glr {
namespace shaders {
GlslShader::GlslShader(std::string source, Type type) : source_(source), type_(type)
{
	shaderId_ = -1;

	//initialize();
}

GlslShader::GlslShader(std::string name, std::string source, Type type, StringBindingsMap bindings) : name_(name), source_(source), type_(type)
{
	shaderId_ = -1;

	// Convert bindings
	for ( auto it = bindings.begin(); it != bindings.end(); ++it )
	{
		bindings_.push_back(IShader::Binding(IShader::parseBindType(it->first), it->second));
		//bindings_[ IShader::parseBindType(it->first) ] = it->second;
		//std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
	}
}

GlslShader::~GlslShader()
{
	glDeleteShader(shaderId_);
}

void GlslShader::compile()
{
	BOOST_LOG_TRIVIAL(debug) << "Compiling shader.";

	if ( shaderId_ < 0 )
	{
		std::string msg("Could not load shader - shader already has an OpenGL id assigned to it.");
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::GlException(msg);
	}


	GLenum shaderType;

	switch ( type_ )
	{
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
		shaderType = GL_GEOMETRY_SHADER;
		break;

	case TYPE_TESSELLATION:
		std::string msg("Could not load shader - tessellation shaders are not yet implemented.");
		BOOST_LOG_TRIVIAL(warning) << msg;
		throw exception::GlException(msg);
	}

	BOOST_LOG_TRIVIAL(debug) << "Shader is of type '" << type_ << "'.";

	shaderId_ = glCreateShader(shaderType);

	const char* source = source_.c_str();

	BOOST_LOG_TRIVIAL(debug) << "source: " << source;

	glShaderSourceARB(shaderId_, 1, &source, nullptr);
	//glShaderSourceARB(fragmentGlslShaderObject, 1, &FragmentShaderSource, &flength);

	glCompileShader(shaderId_);

	GLint compiled;

	glGetProgramiv(shaderId_, GL_COMPILE_STATUS, &compiled);

	// Handle any errors
	if ( compiled = GL_FALSE )
	{
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

IShader::Type GlslShader::getType()
{
	return type_;
}

std::string GlslShader::getName()
{
	return name_;
}

IShader::BindingsMap GlslShader::getBindings()
{
	return bindings_;
}

GLuint GlslShader::getGLShaderId()
{
	return shaderId_;
}

void GlslShader::bind()
{
}
}
}
