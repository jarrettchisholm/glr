/*
 * OpenGlDevice.cpp
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#include <algorithm>

#include <GL/glew.h>

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShader.h"

#include "OpenGlDevice.h"

#include "../common/logger/Logger.h"

#include "MaterialManager.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "AnimationManager.h"

namespace glr {
namespace glw {
	
OpenGlDevice::OpenGlDevice(OpenGlDeviceSettings settings)
{
	initialize( settings );
}

OpenGlDevice::~OpenGlDevice()
{
}
/**
 * Will setup our OpenGlDevice.
 * 
 * @param properties Used to initialize the OpenGlDevice settings
 */
void OpenGlDevice::initialize(OpenGlDeviceSettings settings)
{	
	initializeSettings( settings );
	bufferIds_ = std::vector<GLuint>();
	bindPoints_ = std::vector<GLuint>();
	boundBuffers_ = std::unordered_map<GLuint, GLuint>();
	
	maxNumBindPoints_ = 0;
	currentBindPoint_ = 0;
	
	modelMatrix_ = glm::mat4();
	viewMatrix_ = glm::mat4();
	projectionMatrix_ = glm::mat4();
	
	// Find and set the number of bind points available
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxNumBindPoints_);
	
	for (GLuint i=0; i < maxNumBindPoints_; i++)
	{
		bindPoints_.push_back(i);
	}
	
	bindings_ = std::vector< glmd::int32 >( 1000, -1 );
	
	shaderProgramManager_ = std::unique_ptr< shaders::ShaderProgramManager >(new shaders::ShaderProgramManager(this, true));
	
	materialManager_ = std::unique_ptr<IMaterialManager>( new MaterialManager(this) );
	textureManager_ = std::unique_ptr<ITextureManager>( new TextureManager(this) );
	meshManager_ = std::unique_ptr<IMeshManager>( new MeshManager(this) );
	animationManager_ = std::unique_ptr<IAnimationManager>( new AnimationManager(this) );
}

/**
 * Will override any properties in the OpenGlDevice settings with values set in the parameter settings.
 * 
 * @param settings The settings used to override the OpenGlDevice settings
 */
void OpenGlDevice::initializeSettings(OpenGlDeviceSettings settings)
{
	if ( !settings.defaultTextureDir.empty() )
	{
		settings_.defaultTextureDir = settings.defaultTextureDir;
	}
}

void OpenGlDevice::destroy()
{
}

const glm::mat4& OpenGlDevice::getViewMatrix()
{	
	return viewMatrix_;
}

const glm::mat4& OpenGlDevice::getProjectionMatrix()
{
	return projectionMatrix_;
}

const glm::mat4& OpenGlDevice::getModelMatrix()
{
	return modelMatrix_;
}

void OpenGlDevice::setModelMatrix(const glm::mat4& modelMatrix)
{
	modelMatrix_ = modelMatrix;
}

void OpenGlDevice::setViewMatrix(const glm::mat4& viewMatrix)
{
	viewMatrix_ = viewMatrix;
}

void OpenGlDevice::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
	projectionMatrix_ = projectionMatrix;
}

GLuint OpenGlDevice::createBufferObject(GLenum target, glmd::uint32 totalSize, const void* dataPointer)
{
	GLuint bufferId = 0;
	glGenBuffers(1, &bufferId);
	glBindBuffer(target, bufferId);

	glBufferData(target, totalSize, dataPointer, GL_DYNAMIC_DRAW);
	glBindBuffer(target, 0);
	
	bufferIds_.push_back(bufferId);
	
	return bufferId;
}

void OpenGlDevice::releaseBufferObject(GLuint bufferId)
{
	auto it = std::find(bufferIds_.begin(), bufferIds_.end(), bufferId);
	
	if (it == bufferIds_.end())
	{
		// warning - buffer object not present
		return;
	}	
	
	unbindBuffer( bufferId );
	
	bufferIds_.erase(it);
	glDeleteBuffers(1, &bufferId);
}

GLuint OpenGlDevice::createFrameBufferObject(GLenum target, glmd::uint32 totalSize, const void* dataPointer)
{	
	GLuint bufferId = 0;
	glGenFramebuffers(1, &bufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferId);

	//glBufferData(target, totalSize, dataPointer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_FRAMEBUFFER, 0);
	
	bufferIds_.push_back(bufferId);
	
	return bufferId;
}

void OpenGlDevice::releaseFrameBufferObject(GLuint bufferId)
{
	/*
	auto it = std::find(bufferIds_.begin(), bufferIds_.end(), bufferId);
	
	if (it == bufferIds_.end())
	{
		// warning - buffer object not present
		return;
	}	
	
	unbindBuffer( bufferId );
	
	bufferIds_.erase(it);
	glDeleteBuffers(1, &bufferId);
	*/
}

/**
 * 
 */
void OpenGlDevice::bindBuffer(GLuint bufferId, GLuint bindPoint)
{
	//std::cout << bufferId << " | " << bindPoint << " / " << maxNumBindPoints_ << std::endl;
	assert(bindPoint >= 0);
	assert(bindPoint < maxNumBindPoints_);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, bufferId);
	
	/*
	if (bindings_[bufferId] < 0)
	{
		GLuint bindPoint = bindPoints_[currentBindPoint_];
		currentBindPoint_++;
		
		if ( currentBindPoint_ >= bindPoints_.size() )
			currentBindPoint_ = 0;
			
		bindings_[bufferId] = bindPoint;
		std::cout << bufferId << ": " << bindings_[bufferId] << std::endl;
	}
	
	assert(bindings_[bufferId] >= 0);
	assert(bindings_[bufferId] < maxNumBindPoints_);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, bindings_[bufferId], bufferId);
		
	return bindings_[bufferId];
	*/
	

	// This algorithm was my first attempt at making it more 'efficient' by keeping a cache, and by moving
	// recently used bind points to the bottom of a 'queue' (so they wouldn't be chosen again for a longer
	// period of time).  However, it didn't work :S
	/*
	GLuint bindPoint = 0; 
	 
	// Check if we have bound this buffer already
	auto boundBufferIter = boundBuffers_.find( bufferId );

	if (boundBufferIter != boundBuffers_.end())
	{
		// Pull bind point out of the list and push it on the back
		auto it = std::find( bindPoints_.begin(), bindPoints_.end(), boundBufferIter->second );
		bindPoint = *it;
		bindPoints_.erase( it );
		bindPoints_.push_back( bindPoint );
	} else
	{
		// If we haven't bound it already, use the first available bind point
		bindPoint = bindPoints_[0];

		// Remove any buffers from the bound buffers list that were bound to bindPoint (as it is now used by a different buffer)
		auto it = std::find( bindPoints_.begin(), bindPoints_.end(), bindPoint);
		if ( it != bindPoints_.end() ) 
		{
		    bindPoints_.erase( it );
		}

		// Pop bind point off the top of the list and push it on the back
		bindPoints_.erase( bindPoints_.begin() );
		bindPoints_.push_back( bindPoint );

		// Bind the buffer
		boundBuffers_[bufferId] = bindPoint;
		
	}

	//glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, bufferId);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	return bindPoint;
	*/
}

void OpenGlDevice::unbindBuffer(GLuint bufferId)
{
	auto it = boundBuffers_.find( bufferId );
	if (it != boundBuffers_.end())
	{
		boundBuffers_.erase( it );
	}
}

GLuint OpenGlDevice::getBindPoint()
{
	// TODO: Do I need a better algorithm here?
	GLuint bindPoint = bindPoints_[currentBindPoint_];
	currentBindPoint_++;

	if ( currentBindPoint_ >= bindPoints_.size() )
		currentBindPoint_ = 0;

	assert(bindPoint >= 0);
	assert(bindPoint < maxNumBindPoints_);

	return bindPoint;
}

GlError OpenGlDevice::getGlError()
{
	GlError glErrorObj = GlError();
	
	GLenum glError = glGetError();
	
	if ( glError )
	{
		switch ( glError )
		{
		case GL_INVALID_ENUM:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_ENUM";
			break;

		case GL_INVALID_VALUE:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_VALUE";
			break;

		case GL_INVALID_OPERATION:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_OPERATION";
			break;

		case GL_STACK_OVERFLOW:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_STACK_OVERFLOW";
			break;

		case GL_STACK_UNDERFLOW:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_STACK_UNDERFLOW";
			break;

		case GL_OUT_OF_MEMORY:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_OUT_OF_MEMORY";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_FRAMEBUFFER_OPERATION​";
			break;
		}
	}
	
	return glErrorObj;
}

shaders::IShaderProgramManager* OpenGlDevice::getShaderProgramManager()
{
	return shaderProgramManager_.get();
}

IMaterialManager* OpenGlDevice::getMaterialManager()
{
	return materialManager_.get();
}

ITextureManager* OpenGlDevice::getTextureManager()
{
	return textureManager_.get();
}

IMeshManager* OpenGlDevice::getMeshManager()
{
	return meshManager_.get();
}

IAnimationManager* OpenGlDevice::getAnimationManager()
{
	return animationManager_.get();
}

const OpenGlDeviceSettings& OpenGlDevice::getOpenGlDeviceSettings()
{
	return settings_;
}
}
}
