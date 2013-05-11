/*
 * OpenGlDevice.h
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#ifndef GLRPROGRAM_H_
#define GLRPROGRAM_H_

#include <memory>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

#include "IOpenGlDevice.h"

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShaderProgramBindListener.h"


namespace glr {
namespace glw {
class OpenGlDevice : public IOpenGlDevice {
public:
	OpenGlDevice();
	virtual ~OpenGlDevice();
	
	/* Implementation of IOpenGlDevice methods */
	virtual const glm::mat4& getViewMatrix();
	virtual const glm::mat4& getProjectionMatrix();
	virtual const glm::mat4& getModelMatrix();
	
	virtual void setModelMatrix(glm::mat4& modelMatrix);
	virtual void setViewMatrix(glm::mat4& viewMatrix);
	virtual void setProjectionMatrix(glm::mat4& projectionMatrix);
	
	virtual GLuint createBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer);
	virtual void releaseBufferObject(GLuint bufferId);
	virtual GLuint createFrameBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer);
	virtual void releaseFrameBufferObject(GLuint bufferId);
	virtual GLuint bindBuffer(GLuint bufferId);
	virtual void unbindBuffer(GLuint bufferId);
	
	virtual GlError getGlError();

private:	
	std::vector<GLuint> bufferIds_;
	std::vector<GLuint> bindPoints_;
	std::unordered_map<GLuint, GLuint> boundBuffers_;
	GLint maxNumBindPoints_;
	glmd::uint32 currentBindPoint_;
	
	// Matrices
	glm::mat4 modelMatrix_;
	glm::mat4 viewMatrix_;
	glm::mat4 projectionMatrix_;

	std::unique_ptr< shaders::ShaderProgramManager > shaderProgramManager_;

	void setupUniformBufferObjectBindings(shaders::IShaderProgram* shader);
	void setupLightUbo(std::string name, shaders::IShaderProgram* shader);
	void releaseLightUbo(std::string name);
	void bindUniformBufferObjects(shaders::IShaderProgram* shader);

	void initialize();
	void destroy();
};
}
}
#endif /* GLRPROGRAM_H_ */
