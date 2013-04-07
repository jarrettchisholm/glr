/*
 * GlslShaderProgram.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <algorithm>

#include <boost/log/trivial.hpp>

#include "GlslShaderProgram.h"

namespace glr {
namespace shaders {
GlslShaderProgram::GlslShaderProgram(std::string name, std::vector< std::shared_ptr<GlslShader> > shaders) : name_(name), shaders_(shaders)
{
	programId_ = -1;
}

GlslShaderProgram::~GlslShaderProgram()
{
}

void GlslShaderProgram::compile()
{
	BOOST_LOG_TRIVIAL(debug) << "Initializing shader program.";

	if ( programId_ < 0 )
	{
		BOOST_LOG_TRIVIAL(error) << "Could not load shader program - shader program already has an OpenGL id assigned to it.";
		return;
	}

	// Compile all shaders
	for ( auto s : shaders_ )
	{
		s->compile();
	}

	programId_ = glCreateProgram();

	for ( auto s : shaders_ )
	{
		glAttachShader(programId_, s->getGLShaderId());
	}

	//glBindAttribLocation(programId_, 0, "in_Position");
	//glBindAttribLocation(programId_, 1, "in_Texture");
	//glBindAttribLocation(programId_, 2, "in_Color");

	glLinkProgram(programId_);

	GLint linked;
	glGetProgramiv(programId_, GL_LINK_STATUS, &linked);

	if ( !linked )
	{
		BOOST_LOG_TRIVIAL(error) << "Could not initialize shader program.";

		GLchar errorLog[1024] = { 0 };
		glGetProgramInfoLog(programId_, 1024, nullptr, errorLog);

		BOOST_LOG_TRIVIAL(error) << errorLog;

		return;
	}


	BOOST_LOG_TRIVIAL(debug) << "Done initializing shader program.";
	return;
}

GLuint GlslShaderProgram::getGLShaderProgramId()
{
	return programId_;
}

void GlslShaderProgram::bind()
{
	// Notify all listeners that we are binding this shader program
	for ( auto bindListener : bindListeners_ )
		bindListener->shaderBindCallback( this );

	glUseProgram(programId_);
}

std::string GlslShaderProgram::getName()
{
	return name_;
}

IShader::BindingsMap GlslShaderProgram::getBindings()
{
	IShader::BindingsMap bindings;

	for ( auto s : shaders_ )
	{
		IShader::BindingsMap b = s->getBindings();
		for ( auto e : b )
		{
			bindings.push_back(e);
		}
	}

	return bindings;
}

void GlslShaderProgram::addBindListener(IShaderProgramBindListener* bindListener)
{
	bindListeners_.push_back(bindListener);
}

void GlslShaderProgram::removeBindListener(IShaderProgramBindListener* bindListener)
{
	auto it = std::find(bindListeners_.begin(), bindListeners_.end(), bindListener);

	if ( it != bindListeners_.end())
		bindListeners_.erase(it);
}

void GlslShaderProgram::removeAllBindListeners()
{
	bindListeners_.clear();
}

void GlslShaderProgram::unbindAll()
{
	glUseProgram(0);
}
}
}
