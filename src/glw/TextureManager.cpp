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

#include "../exceptions/Exception.h"


namespace glr {
namespace glw {

TextureManager::TextureManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

TextureManager::~TextureManager()
{
}

Texture2D* TextureManager::getTexture2D(const std::string& name)
{
	if ( textures_.find(name) != textures_.end() )
	{
		LOG_DEBUG( "Texture '" + name + "' found." );
		return textures_[name].get();
	}

	LOG_DEBUG( "Texture '" + name + "' not found." );
	
	return nullptr;
}

Texture2DArray* TextureManager::getTexture2DArray(const std::string& name)
{
	
}

Texture2D* TextureManager::addTexture2D(const std::string& name, const std::string& filename)
{
	LOG_DEBUG( "Loading texture '" + name + "'." );

	if ( textures_.find(name) != textures_.end() && textures_[name].get() != nullptr )
	{
		LOG_DEBUG( "Texture already exists - returning already existing texture." );
		return textures_[name].get();
	}

	const std::string& basepath = openGlDevice_->getOpenGlDeviceSettings().defaultTextureDir;

	LOG_DEBUG( "Loading texture image." );
	utilities::ImageLoader il = utilities::ImageLoader();
	std::unique_ptr<utilities::Image> image = il.loadImageData(basepath + filename);

	if ( image.get() == nullptr )
	{
		std::string msg = std::string( "Unable to load texture: " + filename );
		LOG_ERROR( msg );
		throw exception::Exception( msg );
	}

	return addTexture2D( name, image.get() );
}

Texture2D* TextureManager::addTexture2D(const std::string& name, utilities::Image* image)
{
	LOG_DEBUG( "Loading texture '" + name + "' from image." );

	auto it = textures_.find(name);

	if ( it != textures_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture already exists - returning already existing texture." );
		return it->second.get();
	}
	
	std::stringstream msg;
	msg << "TextureManager::addTexture: image: " << image->width << "x" << image->height;
	LOG_DEBUG( msg.str() );

	LOG_DEBUG( "Creating texture." );
	textures_[name] = std::unique_ptr<Texture2D>(new Texture2D(image, openGlDevice_, name));

	return textures_[name].get();
}

Texture2DArray* TextureManager::addTexture2DArray(const std::string& name, const std::vector<std::string> filenames)
{
	// TODO: implement
	return nullptr;
}

Texture2DArray* TextureManager::addTexture2DArray(const std::string& name, const std::vector<utilities::Image*> images)
{
	// TODO: implement
	return nullptr;
}

}
}
