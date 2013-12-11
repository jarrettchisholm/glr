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

class ITexture {
	
};

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
