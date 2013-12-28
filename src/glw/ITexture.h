#ifndef ITEXTURE_H_
#define ITEXTURE_H_

#include <map>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>

#include "IGraphicsObject.h"

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

class ITexture : public IGraphicsObject {
public:
	virtual ~ITexture()
	{
	}
	;
	
	virtual void bind(GLuint texturePosition = 0) = 0;
};

}
}

#endif /* MESH_H_ */
