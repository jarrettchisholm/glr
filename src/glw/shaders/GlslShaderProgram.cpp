#include <algorithm>
#include <utility>

#include "glw/shaders/GlslShaderProgram.hpp"

#include "common/logger/Logger.hpp"

#include "exceptions/GlException.hpp"
#include "exceptions/InvalidArgumentException.hpp"

namespace glr
{
namespace shaders
{

GlslShaderProgram::GlslShaderProgram(std::string name, std::vector< std::unique_ptr<GlslShader> > shaders, glw::IOpenGlDevice* openGlDevice)
	: name_(std::move(name)), shaders_(std::move(shaders)), openGlDevice_(openGlDevice)
{
	programId_ = -1;
	
	this->addBindListener(openGlDevice_);
}

GlslShaderProgram::~GlslShaderProgram()
{
	if (programId_ >= 0)
	{
		glDeleteProgram(programId_);
	}
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
	for ( auto& s : shaders_ )
	{
		s->compile();
	}

	programId_ = glCreateProgram();

	for ( auto& s : shaders_ )
	{
		glAttachShader(programId_, s->getGlShaderId());
	}
	
	generateBindings();

	// Note: reservedVertexAttribLocations is defined in IShader.hpp
	for ( auto& it : reservedVertexAttribLocations )
	{
		glBindAttribLocation(programId_, it.second, it.first.c_str());
	}
	
	// Bind all shader defined attribute locations
	for ( auto& b : bindings_ )
	{
		if (b.type == IShader::BindType::BIND_TYPE_LOCATION)
		{
			glBindAttribLocation(programId_, b.bindPoint, b.variableName.c_str());
		}
	}
	
	
	glLinkProgram(programId_);

	GLint linked = GL_FALSE;
	glGetProgramiv(programId_, GL_LINK_STATUS, &linked);

	if ( linked == GL_FALSE )
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
		for ( auto& s : shaders_ )
		{
			glDetachShader(programId_, s->getGlShaderId());
		}
		glDeleteProgram(programId_);
		
		LOG_ERROR( ss.str() );
		throw exception::GlException(ss.str());
	}

	LOG_DEBUG( "Done initializing shader program '" + name_ + "'." );
}

GLuint GlslShaderProgram::getGLShaderProgramId() const
{
	return programId_;
}

void GlslShaderProgram::bind()
{
	assert(programId_ >= 0);
	
	// Don't bind if we are already bound
	if (openGlDevice_->getCurrentlyBoundShaderProgram() == this)
	{
		return;
	}
	
	glUseProgram(programId_);

	// We need to invalidate the current bind points so that we can rebind what we need to
	// to this shader program.
	openGlDevice_->invalidateBindPoints();
	
	// Bind all the variables to bind points
	for ( auto& b : bindings_ )
	{
		// Ignore location bind types (they are set before the shader program is linked)
		if (b.type == IShader::BindType::BIND_TYPE_LOCATION)
			continue;

		b.bindPoint = openGlDevice_->getBindPoint();

		GLuint uniformBlockIndex = glGetUniformBlockIndex(programId_,  b.variableName.c_str());
		if ( uniformBlockIndex != GL_INVALID_INDEX )
		{
			glUniformBlockBinding(programId_, uniformBlockIndex, b.bindPoint);
		}
		else
		{
			LOG_WARN( std::string("Unable to find block index for variable '" + b.variableName + "' in shader program '" + name_ + "'.") );
		}
	}
	
	// Notify all listeners that we have bound this shader program
	for ( auto bindListener : bindListeners_ )
	{
		bindListener->shaderBindCallback( this );
	}
}

GLint GlslShaderProgram::getBindPointByBindingName(IShader::BindType bindType) const
{
	for ( auto& b : bindings_ )
	{
		if (b.type == bindType)
		{
			return b.bindPoint;
		}
	}
	
	return -1;
}

GLint GlslShaderProgram::getBindPointByVariableName(const std::string& varName) const
{
	for ( auto& b : bindings_ )
	{
		if (b.variableName == varName)
		{
			return b.bindPoint;
		}
	}
	
	return -1;
}

GLint GlslShaderProgram::getVertexAttributeLocationByName(const std::string& varName) const
{
	GLint pos = glGetAttribLocation(programId_, varName.c_str());
	
	return pos;
}

const std::string& GlslShaderProgram::getName() const
{
	return name_;
}

void GlslShaderProgram::generateBindings()
{
	bindings_ = IShader::BindingsMap();

	for ( auto& s : shaders_ )
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
	if (bindListener == nullptr)
	{
		std::string msg = std::string( "Bind listener must not be null." );
		LOG_ERROR( msg );
		throw exception::InvalidArgumentException(msg);
	}
	
	bindListeners_.push_back(bindListener);
}

void GlslShaderProgram::removeBindListener(IShaderProgramBindListener* bindListener)
{
	if (bindListener == nullptr)
	{
		std::string msg = std::string( "Bind listener must not be null." );
		LOG_ERROR( msg );
		throw exception::InvalidArgumentException(msg);
	}
	
	auto it = std::find(bindListeners_.begin(), bindListeners_.end(), bindListener);

	if ( it != bindListeners_.end())
	{
		bindListeners_.erase(it);
	}
}

void GlslShaderProgram::removeAllBindListeners()
{
	bindListeners_.clear();
}

void GlslShaderProgram::unbind()
{
	openGlDevice_->unbindAllShaderPrograms();
	
	// Notify all listeners that we have bound no shader program
	for ( auto bindListener : bindListeners_ )
	{
		bindListener->shaderBindCallback( nullptr );
	}
}

}
}
