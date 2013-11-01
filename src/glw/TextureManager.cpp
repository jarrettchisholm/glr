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

#include "../common/utilities/ImageLoader.h"


namespace glr {
namespace glw {
TextureManager::TextureManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

TextureManager::~TextureManager()
{
}

Texture* TextureManager::getTexture(const std::string name)
{
	if ( textures_.find(name) != textures_.end() )
	{
		BOOST_LOG_TRIVIAL(debug) << "Texture found.";
		return textures_[name].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Texture not found.";
	
	return nullptr;
}

Texture* TextureManager::addTexture(const std::string name, const std::string filename)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading texture...";

	if ( textures_.find(name) != textures_.end() && textures_[name].get() != nullptr )
	{
		BOOST_LOG_TRIVIAL(debug) << "Texture already exists.";
		return textures_[name].get();
	}

	std::string basepath = openGlDevice_->getOpenGlDeviceSettings().defaultTextureDir;

	BOOST_LOG_TRIVIAL(debug) << "Loading texture image.";
	utilities::ImageLoader il = utilities::ImageLoader();
	std::unique_ptr<utilities::Image> image = il.loadImageData(basepath + filename);

	if ( image.get() == nullptr )
	{
		BOOST_LOG_TRIVIAL(debug) << "Unable to load texture: " << filename;
		return nullptr;
	}

	BOOST_LOG_TRIVIAL(debug) << "TextureManager::addTexture: image: " << image->width << "x" << image->height;

	BOOST_LOG_TRIVIAL(debug) << "Creating texture.";
	textures_[name] = std::unique_ptr<Texture>(new Texture(image.get(), openGlDevice_, name));

	return textures_[name].get();
}

}
}
