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

#include "IOpenGlDevice.h"

#include "ITextureManager.h"

#include "Texture.h"

namespace glr {
namespace glw {
class TextureManager : public ITextureManager {
public:
	TextureManager(IOpenGlDevice* openGlDevice);
	virtual ~TextureManager();

	virtual Texture* getTexture(const std::string filename);
	virtual Texture* addTexture(const std::string filename);
	
private:
	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Texture> > textures_;
};
}
}
#endif /* TEXTUREMANAGER_H_ */
