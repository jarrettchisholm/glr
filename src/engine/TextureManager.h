/*
 * TextureManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <memory>
#include <map>
#include <string>

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include "../common/compatibility/Types.h"

#include "Texture.h"

namespace oglre {

namespace engine {
	
using namespace compatibility;

class TextureManager {
public:
	TextureManager();
	virtual ~TextureManager();
	TextureManager(TextureManager const&);
	TextureManager& operator=(TextureManager const&);
	
	static TextureManager* getInstance();

	Texture* getTexture(const std::string filename);
	
private:
	static TextureManager* textureManager_;

	std::map< std::string, std::unique_ptr<Texture> > textures_;
	
};

}

}

#endif /* TEXTUREMANAGER_H_ */
