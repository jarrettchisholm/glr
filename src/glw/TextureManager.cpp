/*
 * TextureManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "TextureManager.h"
#include "Constants.h"

#include "../common/utilities/ImageLoader.h"


namespace glr {
namespace glw {
TextureManager::TextureManager(IOpenGlDevice* openGlDevice)
	: openGlDevice_(openGlDevice)
{
}

TextureManager::~TextureManager()
{
}

Texture* TextureManager::getTexture(const std::string filename)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading texture...";

	if ( textures_[filename] != 0 )
	{
		BOOST_LOG_TRIVIAL(debug) << "Texture found.";
		return textures_[filename].get();
	}

	std::string basepath = Constants::MODEL_DIRECTORY;

	BOOST_LOG_TRIVIAL(debug) << "Loading texture image.";
	utilities::ImageLoader il;
	utilities::Image* image = il.loadImageData(basepath + filename);

	if ( image == 0 )
	{
		BOOST_LOG_TRIVIAL(debug) << "Unable to load texture.";
		return 0;
	}

	BOOST_LOG_TRIVIAL(debug) << "TextureManager::getTexture: image: " << image->width << "x" << image->height;

	BOOST_LOG_TRIVIAL(debug) << "Creating texture.";
	textures_[filename] = std::unique_ptr<Texture>(new Texture(image, openGlDevice_));

	delete image;

	return textures_[filename].get();
}
}
}
