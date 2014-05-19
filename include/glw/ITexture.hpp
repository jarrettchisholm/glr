#ifndef ITEXTURE_H_
#define ITEXTURE_H_

#include <map>
#include <iostream>
#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IGraphicsObject.hpp"

#include "common/logger/Logger.hpp"
#include "common/utilities/ImageLoader.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

/**
 * Used to pass in TextureSettings settings (so we don't have to have a method with a whole ton of parameters).
 */
struct TextureSettings
{
	TextureSettings() : textureWrapS(GL_CLAMP_TO_EDGE), textureWrapT(GL_CLAMP_TO_EDGE)
	{
	}
	
	GLint textureWrapS;
	GLint textureWrapT;
};

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
	 * @param texturePosition The texture position to bind to.  This must be a value between 0 - 9.
	 */
	virtual void bind(GLuint texturePosition = 0) const = 0;
	
	/**
	 * Returns the name of this texture.
	 * 
	 * @return The name of the texture.
	 */
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* MESH_H_ */
