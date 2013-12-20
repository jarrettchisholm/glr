#ifndef ITEXTURE_H_
#define ITEXTURE_H_

#include <map>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>

#include "../common/logger/Logger.h"
#include "../common/utilities/ImageLoader.h"

namespace glr {
namespace glw {

/**
 * Used to pass in TextureSettings settings (so we don't have to have a method with a whole ton of parameters).
 */
struct TextureSettings {
	TextureSettings() : textureWrapS(GL_CLAMP_TO_EDGE), textureWrapT(GL_CLAMP_TO_EDGE)
	{
	}
	
	GLint textureWrapS;
	GLint textureWrapT;
};

class ITexture {
public:
	virtual ~ITexture()
	{
	}
	;
	
	virtual void bind(GLuint texturePosition = 0) = 0;
};

/**
 * Will return the OpenGL compatible format of the given image Format 'format'.
 * 
 * If no known compatible OpenGL format is found, FORMAT_UNKNOWN is returned.
 * 
 * @param format
 * 
 * @return The OpenGL compatible format of the given image Format 'format', or FORMAT_UNKNOWN if no known compatible OpenGL format is found.
 */
static GLint getInternalFormat( utilities::Format format )
{	
	switch (format)
	{
		case utilities::Format::FORMAT_RGB:
			return GL_RGB;
		
		case utilities::Format::FORMAT_RGBA:
			return GL_RGBA;
	}
	
	return (GLint)utilities::Format::FORMAT_UNKNOWN;
}

}
}

#endif /* MESH_H_ */
