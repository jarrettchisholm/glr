#ifndef IOPENGLDEVICE_H_
#define IOPENGLDEVICE_H_

#include <string>

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "ITextureBindListener.hpp"
#include "IMaterialBindListener.hpp"

#include "shaders/IShaderProgramManager.hpp"
#include "shaders/IShaderProgramBindListener.hpp"

#include "glw/OpenGlDeviceSettings.hpp"
#include "glw/Constants.hpp"

namespace glr
{

namespace shaders
{
	class IShaderProgram;
}

namespace glw
{

class IMeshManager;
class ITextureManager;
class IMaterialManager;
class IAnimationManager;

struct GlError
{
	GLenum type;
	std::string name;
	
	/**
	 * Default constructor (only doing this so that I can have default values for the member variables)
	 */
	GlError() : type(GL_NONE), name(std::string(""))
	{
	}	
};

/**
 * 
 */
class IOpenGlDevice 
	: public virtual glr::shaders::IShaderProgramBindListener,
	public virtual ITextureBindListener,
	public virtual IMaterialBindListener
{
public:
	virtual ~IOpenGlDevice()
	{
	}
	;

	virtual GLuint createBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer, GLenum usage = GL_DYNAMIC_DRAW) = 0;
	virtual void releaseBufferObject(GLuint bufferId) = 0;
	virtual GLuint createFrameBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer) = 0;
	virtual void releaseFrameBufferObject(GLuint bufferId) = 0;
	virtual void bindBuffer(GLuint bufferId, GLuint bindPoint) = 0;
	virtual void unbindBuffer(GLuint bufferId) = 0;
	virtual GLuint getBindPoint() = 0;
	
	/**
	 * Will 'invalidate' all active bind points, which effectively means that the next call to `getBindPoint()` will return
	 * the first bind point, and any buffers that are currently attached to a bind point must no longer be considered active.
	 * 
	 * This method is typically called when we are binding a new shader program.
	 */
	virtual void invalidateBindPoints() = 0;
	
	/**
	 * Returns the maximum number of bind points available for use.
	 */
	virtual glm::detail::uint32 getMaximumNumberOfBindPoints() = 0;
	
	virtual GlError getGlError() = 0;
	
	/* Getters */
	virtual glr::shaders::IShaderProgramManager* getShaderProgramManager() = 0;
	
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
	
	virtual const OpenGlDeviceSettings& getOpenGlDeviceSettings() = 0;
	
	/**
	 * Add a listener, which will be notified when this texture gets bound.
	 * 
	 * @param bindListener A pointer to an object implementing the IShaderProgramBindListener interface.
	 */
	virtual void addBindListener(shaders::IShaderProgramBindListener* bindListener) = 0;
	
	/**
	 * Remove a bind listener.
	 * 
	 * @param bindListener A pointer to an IShaderProgramBindListener object that should be removed as a bind listener for this shader.
	 */
	virtual void removeBindListener(shaders::IShaderProgramBindListener* bindListener) = 0;
	
	/**
	 * Add a listener, which will be notified when this texture gets bound.
	 * 
	 * @param bindListener A pointer to an object implementing the ITextureBindListener interface.
	 */
	virtual void addBindListener(ITextureBindListener* bindListener) = 0;
	
	/**
	 * Remove a bind listener.
	 * 
	 * @param bindListener A pointer to an ITextureBindListener object that should be removed as a bind listener for this texture.
	 */
	virtual void removeBindListener(ITextureBindListener* bindListener) = 0;
	
	/**
	 * Add a listener, which will be notified when this material gets bound.
	 * 
	 * @param bindListener A pointer to an object implementing the IMaterialBindListener interface.
	 */
	virtual void addBindListener(IMaterialBindListener* bindListener) = 0;
	
	/**
	 * Remove a bind listener.
	 * 
	 * @param bindListener A pointer to an IMaterialBindListener object that should be removed as a bind listener for this material.
	 */
	virtual void removeBindListener(IMaterialBindListener* bindListener) = 0;
	
	virtual void unbindAllShaderPrograms() = 0;
	virtual void unbindAllTextures() = 0;
	virtual void unbindAllMaterials() = 0;
	virtual shaders::IShaderProgram* getCurrentlyBoundShaderProgram() const = 0;
	virtual IMaterial* getCurrentlyBoundMaterial() const = 0;
	virtual ITexture* getCurrentlyBoundTexture() const = 0;
};

}
}

#endif /* IOPENGLDEVICE_H_ */
