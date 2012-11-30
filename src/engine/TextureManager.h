/*
 * TextureManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <string>

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include "../common/compatibility/Types.h"

#include "Texture.h"

namespace icee {

namespace engine {
	
using namespace compatibility;

class TextureManager {
public:
	TextureManager();
	virtual ~TextureManager();

	Texture* loadTexture(const std::string filename);
	
private:
	
};

}

}

#endif /* TEXTUREMANAGER_H_ */
