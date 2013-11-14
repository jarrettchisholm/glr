/*
 * TextureManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <sstream>

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
		LOG_DEBUG( "Texture '" + name + "' found." );
		return textures_[name].get();
	}

	LOG_DEBUG( "Texture '" + name + "' not found." );
	
	return nullptr;
}

Texture* TextureManager::addTexture(const std::string name, const std::string filename)
{
	LOG_DEBUG( "Loading texture '" + name + "'." );

	if ( textures_.find(name) != textures_.end() && textures_[name].get() != nullptr )
	{
		LOG_DEBUG( "Texture already exists." );
		return textures_[name].get();
	}

	std::string basepath = openGlDevice_->getOpenGlDeviceSettings().defaultTextureDir;

	LOG_DEBUG( "Loading texture image." );
	utilities::ImageLoader il = utilities::ImageLoader();
	std::unique_ptr<utilities::Image> image = il.loadImageData(basepath + filename);

	if ( image.get() == nullptr )
	{
		LOG_WARN( "Unable to load texture: " + filename );
		return nullptr;
	}

	std::stringstream msg;
	msg << "TextureManager::addTexture: image: " << image->width << "x" << image->height;
	LOG_DEBUG( msg.str() );

	LOG_DEBUG( "Creating texture." );
	textures_[name] = std::unique_ptr<Texture>(new Texture(image.get(), openGlDevice_, name));

	return textures_[name].get();
}

}
}
