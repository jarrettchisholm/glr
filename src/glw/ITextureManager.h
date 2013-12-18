/*
 * ITextureManager.h
 *
 *  Created on: 2013-04-13
 *      Author: jarrett
 */

#ifndef ITEXTUREMANAGER_H_
#define ITEXTUREMANAGER_H_

#include <string>

#include "Texture2D.h"
#include "Texture2DArray.h"

#include "ITexture.h"

#include "../common/utilities/ImageLoader.h"

namespace glr {
namespace glw {
	
class Texture2D;
class Texture2DArray;
	
class ITextureManager {
public:
	virtual ~ITextureManager()
	{
	}
	;

	//virtual Texture1D* getTexture1D(const std::string name) = 0;
	//virtual Texture1DArray* getTexture1DArray(const std::string name) = 0;
	//virtual Texture1D* addTexture1D(const std::string name, const std::string filename) = 0;
	//virtual Texture1DArray* addTexture1DArray(const std::string name, const std::string filename) = 0;

	virtual Texture2D* getTexture2D(const std::string& name) = 0;
	virtual Texture2DArray* getTexture2DArray(const std::string& name) = 0;
	
	virtual Texture2D* addTexture2D(const std::string& name, const std::string& filename, const TextureSettings settings = TextureSettings()) = 0;
	virtual Texture2D* addTexture2D(const std::string& name, utilities::Image* image, const TextureSettings settings = TextureSettings()) = 0;
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const std::vector<std::string> filenames) = 0;
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const std::vector<utilities::Image*> images) = 0;
	// Do I want to do this one?
	//virtual Texture2DArray* addTexture2DArray(const std::string name, const std::vector<Texture2D*> textures) = 0;
	
	//virtual Texture3D* getTexture3D(const std::string name) = 0;
	//virtual Texture3D* addTexture3D(const std::string name, const std::string filename) = 0;
};

}
}
#endif /* ITEXTUREMANAGER_H_ */
