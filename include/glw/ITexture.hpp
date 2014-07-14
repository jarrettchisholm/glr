#ifndef ITEXTURE_H_
#define ITEXTURE_H_

#include <map>
#include <iostream>
#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IGraphicsObject.hpp"
#include "ITextureBindListener.hpp"
#include "TextureSettings.hpp"

#include "common/logger/Logger.hpp"
#include "common/utilities/ImageLoader.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

class ITexture : public virtual IGraphicsObject, public virtual serialize::ITextSerializable
{
public:
	virtual ~ITexture()
	{
	}
	;
	
	/**
	 * Binds the data in OpenGL, making it ready to use.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 * 
	 * @param texturePosition The texture position to bind to.  This must be a value between 0 - 9.
	 */
	virtual void bind(GLuint texturePosition = 0) = 0;
	
	/**
	 * Returns the name of this texture.
	 * 
	 * @return The name of the texture.
	 */
	virtual const std::string& getName() const = 0;
	
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
};

}
}

#endif /* MESH_H_ */
