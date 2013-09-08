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
	glDeleteProgram(programId_);
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

	// TODO: make these not hard coded?
	/*
	glBindAttribLocation(programId_, 0, "in_Position");
	glBindAttribLocation(programId_, 1, "in_Texture");
	glBindAttribLocation(programId_, 2, "in_Color");
	glBindAttribLocation(programId_, 3, "in_BoneIds");
	glBindAttribLocation(programId_, 4, "in_BoneWeights");
	*/
	
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
	
	
	generateBindings();

	BOOST_LOG_TRIVIAL(debug) << "Done initializing shader program.";
	return;
}

GLuint GlslShaderProgram::getGLShaderProgramId()
{
	return programId_;
}

void GlslShaderProgram::bind()
{
	glUseProgram(programId_);
	
	// Notify all listeners that we have bound this shader program
	for ( auto bindListener : bindListeners_ )
		bindListener->shaderBindCallback( this );
}

void GlslShaderProgram::bindVariable(std::string varName, GLuint bindPoint)
{
	GLuint uniformBlockIndex = glGetUniformBlockIndex(programId_, varName.c_str());
	glUniformBlockBinding(programId_, uniformBlockIndex, bindPoint);

	
	//glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	//glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, ubo);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlslShaderProgram::bindVariableByBindingName(IShader::BindType bindType, GLuint bindPoint)
{
	for ( auto it = bindings_.begin(); it != bindings_.end(); it++)
	{
		if (it->first == bindType)
		{
			GLuint uniformBlockIndex = glGetUniformBlockIndex(programId_,  it->second.c_str());
			glUniformBlockBinding(programId_, uniformBlockIndex, bindPoint);
		}
	}
	
	//glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	//glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, ubo);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::string GlslShaderProgram::getName()
{
	return name_;
}

void GlslShaderProgram::generateBindings()
{
	bindings_ = IShader::BindingsMap();

	for ( auto s : shaders_ )
	{
		IShader::BindingsMap b = s->getBindings();
		for ( auto e : b )
		{
			bindings_.push_back(e);
		}
	}
}

IShader::BindingsMap GlslShaderProgram::getBindings()
{
	return bindings_;
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
