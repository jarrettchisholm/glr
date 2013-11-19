/*
 * GlslShaderProgram.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <algorithm>

#include "GlslShaderProgram.h"

#include "../../common/logger/Logger.h"

#include "../../exceptions/GlException.h"

namespace glr {
namespace shaders {

GlslShaderProgram::GlslShaderProgram(std::string name, std::vector< std::shared_ptr<GlslShader> > shaders, glw::IOpenGlDevice* openGlDevice) : name_(name), shaders_(shaders), openGlDevice_(openGlDevice)
{
	programId_ = -1;
}

GlslShaderProgram::~GlslShaderProgram()
{
	glDeleteProgram(programId_);
}

void GlslShaderProgram::compile()
{
	LOG_DEBUG( "Initializing shader program '" + name_ + "'." );

	if ( programId_ < 0 )
	{		
		std::string msg = std::string( "Could not load shader program '" + name_ + "' - shader program already has an OpenGL id assigned to it." );
		LOG_ERROR( msg );
		throw exception::GlException(msg);
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
	// NOTE: I need these in windows for some reason...?
	glBindAttribLocation(programId_, 0, "in_Position");
	glBindAttribLocation(programId_, 1, "in_Texture");
	glBindAttribLocation(programId_, 2, "in_Normal");
	glBindAttribLocation(programId_, 3, "in_Color");
	glBindAttribLocation(programId_, 4, "in_BoneIds");
	glBindAttribLocation(programId_, 5, "in_BoneWeights");
	
	
	glLinkProgram(programId_);

	GLint linked;
	glGetProgramiv(programId_, GL_LINK_STATUS, &linked);

	if ( !linked )
	{
		std::stringstream ss;
		ss << "Could not initialize shader program '" << name_ << "'.";
		
		GLint infoLogLength;
		glGetProgramiv(programId_, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(programId_, infoLogLength, nullptr, strInfoLog);

		ss << "\ncompiler_log: " << strInfoLog;

		delete[] strInfoLog;
		
		// Cleanup
		for ( auto s : shaders_ )
		{
			glDetachShader(programId_, s->getGLShaderId());
		}
		glDeleteProgram(programId_);
		
		LOG_ERROR( ss.str() );
		throw exception::GlException(ss.str());
	}
	
	
	generateBindings();

	LOG_DEBUG( "Done initializing shader program '" + name_ + "'." );
}

GLuint GlslShaderProgram::getGLShaderProgramId()
{
	return programId_;
}

void GlslShaderProgram::bind()
{
	glUseProgram(programId_);

	// We need to invalidate the current bind points so that we can rebind what we need to
	// to this shader program.
	openGlDevice_->invalidateBindPoints();
	
	// Bind all the variables to bind points
	for ( auto& b : bindings_ )
	{
		b.bindPoint = openGlDevice_->getBindPoint();

		GLuint uniformBlockIndex = glGetUniformBlockIndex(programId_,  b.variableName.c_str());
		glUniformBlockBinding(programId_, uniformBlockIndex, b.bindPoint);
	}
	
	// Notify all listeners that we have bound this shader program
	for ( auto bindListener : bindListeners_ )
		bindListener->shaderBindCallback( this );
}

GLuint GlslShaderProgram::getBindPointByBindingName(IShader::BindType bindType)
{
	for ( auto& b : bindings_ )
	{
		if (b.type == bindType)
		{
			return b.bindPoint;
		}
	}
	// Didn't find binding type in bind map
	LOG_WARN( "Unable to find binding for bind type: " << bindType );
	
	return -1;
}

GLuint GlslShaderProgram::getBindPointByVariableName(std::string varName)
{
	for ( auto& b : bindings_ )
	{
		if (b.variableName == varName)
		{
			return b.bindPoint;
		}
	}
	
	// Didn't find binding type in bind map
	LOG_WARN( "Unable to find binding for variable name: " << varName );
	
	return -1;
}

/*
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
*/

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
