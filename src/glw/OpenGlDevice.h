#ifndef OPENGLDEVICE_H_
#define OPENGLDEVICE_H_

#include <memory>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

#include "IOpenGlDevice.h"

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShaderProgramBindListener.h"

namespace glmd = glm::detail;

namespace glr
{
namespace glw
{
	
class OpenGlDevice : public IOpenGlDevice
{
public:
	OpenGlDevice(OpenGlDeviceSettings settings);
	virtual ~OpenGlDevice();
	
	/* Implementation of IOpenGlDevice methods */
	virtual const glm::mat4& getViewMatrix();
	virtual const glm::mat4& getProjectionMatrix();
	virtual const glm::mat4& getModelMatrix();
	
	virtual void setModelMatrix(const glm::mat4& modelMatrix);
	virtual void setViewMatrix(const glm::mat4& viewMatrix);
	virtual void setProjectionMatrix(const glm::mat4& projectionMatrix);
	
	virtual GLuint createBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer);
	virtual void releaseBufferObject(GLuint bufferId);
	virtual GLuint createFrameBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer);
	virtual void releaseFrameBufferObject(GLuint bufferId);
	virtual void bindBuffer(GLuint bufferId, GLuint bindPoint);
	virtual void unbindBuffer(GLuint bufferId);
	virtual GLuint getBindPoint();
	
	virtual void invalidateBindPoints();
	virtual glm::detail::uint32 getMaximumNumberOfBindPoints();
	
	virtual GlError getGlError();
	
	virtual shaders::IShaderProgramManager* getShaderProgramManager();
	
	virtual IMaterialManager* getMaterialManager();
	virtual ITextureManager* getTextureManager();
	virtual IMeshManager* getMeshManager();
	virtual IAnimationManager* getAnimationManager();
	
	virtual const OpenGlDeviceSettings& getOpenGlDeviceSettings();

private:	
	std::vector<GLuint> bufferIds_;
	std::vector<GLuint> bindPoints_;
	std::unordered_map<GLuint, GLuint> boundBuffers_;
	GLint maxNumBindPoints_;
	glmd::uint32 currentBindPoint_;
	//std::vector< glmd::int32 > bindings_;
	
	std::unique_ptr<IMaterialManager> materialManager_;
	std::unique_ptr<ITextureManager> textureManager_;
	std::unique_ptr<IMeshManager> meshManager_;
	std::unique_ptr<IAnimationManager> animationManager_;
	
	// Matrices
	glm::mat4 modelMatrix_;
	glm::mat4 viewMatrix_;
	glm::mat4 projectionMatrix_;

	std::unique_ptr< shaders::ShaderProgramManager > shaderProgramManager_;
	
	OpenGlDeviceSettings settings_;

	void setupUniformBufferObjectBindings(shaders::IShaderProgram* shader);
	void setupLightUbo(std::string name, shaders::IShaderProgram* shader);
	void releaseLightUbo(std::string name);
	void bindUniformBufferObjects(shaders::IShaderProgram* shader);

	void initialize(OpenGlDeviceSettings settings);
	void initializeSettings(OpenGlDeviceSettings settings);
	void destroy();
};

}
}

#endif /* OPENGLDEVICE_H_ */
