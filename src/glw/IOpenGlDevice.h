/*
 * IOpenGlDevice.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IOPENGLDEVICE_H_
#define IOPENGLDEVICE_H_

#include <string>

#include <GL/glew.h>

#include "glm/glm.hpp"

#include "shaders/IShaderProgramManager.h"

#include "IMaterialManager.h"
#include "ITextureManager.h"
#include "IMeshManager.h"
#include "IAnimationManager.h"

namespace glr {
namespace glw {

class IMeshManager;

struct GlError
{
	GLenum type = GL_NONE;
	std::string name = "";
};

class IOpenGlDevice {
public:
	virtual ~IOpenGlDevice()
	{
	}
	;

	virtual GLuint createBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer) = 0;
	virtual void releaseBufferObject(GLuint bufferId) = 0;
	virtual GLuint createFrameBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer) = 0;
	virtual void releaseFrameBufferObject(GLuint bufferId) = 0;
	virtual GLuint bindBuffer(GLuint bufferId) = 0;
	virtual void unbindBuffer(GLuint bufferId) = 0;
	
	virtual GlError getGlError() = 0;
	
	/* Getters */
	virtual shaders::IShaderProgramManager* getShaderProgramManager() = 0;
	
	virtual IMaterialManager* getMaterialManager() = 0;
	virtual ITextureManager* getTextureManager() = 0;
	virtual IMeshManager* getMeshManager() = 0;
	virtual IAnimationManager* getAnimationManager() = 0;
	
	// Matrix data
	virtual const glm::mat4& getViewMatrix() = 0;
	virtual const glm::mat4& getProjectionMatrix() = 0;
	virtual const glm::mat4& getModelMatrix() = 0;
	
	virtual void setModelMatrix(const glm::mat4& modelMatrix) = 0;
	virtual void setViewMatrix(const glm::mat4& viewMatrix) = 0;
	virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) = 0;
};

}
}
#endif /* IOPENGLDEVICE_H_ */
