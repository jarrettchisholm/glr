#ifdef _WIN32
#include <windows.h>
#endif

#include <sstream>

#include "TextureManager.h"

#include "../exceptions/Exception.h"


namespace glr
{
namespace glw
{

TextureManager::TextureManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

TextureManager::~TextureManager()
{
}

Texture2D* TextureManager::getTexture2D(const std::string& name)
{
	auto it = textures2D_.find(name);
	if ( it != textures2D_.end() )
	{
		LOG_DEBUG( "Texture '" + name + "' found." );
		return it->second.get();
	}

	LOG_DEBUG( "Texture '" + name + "' not found." );
	
	return nullptr;
}

Texture2DArray* TextureManager::getTexture2DArray(const std::string& name)
{
	auto it = textures2DArray_.find(name);
	if ( it != textures2DArray_.end() )
	{
		LOG_DEBUG( "Texture 2D array '" + name + "' found." );
		return it->second.get();
	}

	LOG_DEBUG( "Texture 2D array '" + name + "' not found." );
	
	return nullptr;
}

Texture2D* TextureManager::addTexture2D(const std::string& name, const TextureSettings settings)
{
	LOG_DEBUG( "Loading texture 2d'" + name + "'." );

	auto it = textures2D_.find(name);

	if ( it != textures2D_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture already exists - returning already existing texture." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating texture 2d." );
	textures2D_[name] = std::unique_ptr<Texture2D>(new Texture2D(openGlDevice_, name, settings));

	return textures2D_[name].get();
}

Texture2D* TextureManager::addTexture2D(const std::string& name, const std::string& filename, const TextureSettings settings)
{
	LOG_DEBUG( "Loading texture 2d'" + name + "'." );

	auto it = textures2D_.find(name);

	if ( it != textures2D_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture already exists - returning already existing texture." );
		return it->second.get();
	}

	const std::string& basepath = openGlDevice_->getOpenGlDeviceSettings().defaultTextureDir;

	LOG_DEBUG( "Loading texture 2d image." );
	utilities::ImageLoader il = utilities::ImageLoader();
	std::unique_ptr<utilities::Image> image = il.loadImageData(basepath + filename);

	if ( image.get() == nullptr )
	{
		std::string msg = std::string( "Unable to load texture: " + filename );
		LOG_ERROR( msg );
		throw exception::Exception( msg );
	}

	return addTexture2D( name, image.get(), settings );
}

Texture2D* TextureManager::addTexture2D(const std::string& name, utilities::Image* image, const TextureSettings settings)
{
	LOG_DEBUG( "Loading texture 2d '" + name + "' from image." );

	if ( image == nullptr )
	{
		std::string msg = std::string( "Unable to load texture: " + name );
		LOG_ERROR( msg );
		throw exception::Exception( msg );
	}

	auto it = textures2D_.find(name);

	if ( it != textures2D_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture already exists - returning already existing texture." );
		return it->second.get();
	}
	
	std::stringstream msg;
	msg << "TextureManager::addTexture2D: image: " << image->width << "x" << image->height;
	LOG_DEBUG( msg.str() );

	LOG_DEBUG( "Creating texture 2d." );
	textures2D_[name] = std::unique_ptr<Texture2D>(new Texture2D(image, openGlDevice_, name, settings));

	return textures2D_[name].get();
}

Texture2DArray* TextureManager::addTexture2DArray(const std::string& name, const TextureSettings settings)
{
	LOG_DEBUG( "Loading texture 2d array '" + name + "'." );

	auto it = textures2DArray_.find(name);

	if ( it != textures2DArray_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture 2d array already exists - returning already existing texture 2d array." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating texture 2d array." );
	textures2DArray_[name] = std::unique_ptr<Texture2DArray>(new Texture2DArray(openGlDevice_, name, settings));

	return textures2DArray_[name].get();
}

Texture2DArray* TextureManager::addTexture2DArray(const std::string& name, const std::vector<std::string>& filenames, const TextureSettings settings)
{
	LOG_DEBUG( "Loading texture 2d array '" + name + "'." );

	auto it = textures2DArray_.find(name);

	if ( it != textures2DArray_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture 2d array already exists - returning already existing texture 2d array." );
		return it->second.get();
	}

	const std::string& basepath = openGlDevice_->getOpenGlDeviceSettings().defaultTextureDir;

	LOG_DEBUG( "Loading texture image." );
	utilities::ImageLoader il = utilities::ImageLoader();
	
	auto images = std::vector< std::unique_ptr<utilities::Image> >();
	for (auto& s : filenames)
	{
		auto image = il.loadImageData(basepath + s);
		if (image.get() == nullptr)
		{
			// Cleanup
			images.clear();
			std::string msg = std::string( "Unable to load texture for texture 2d array: " + s );
			LOG_ERROR( msg );
			throw exception::Exception( msg );
		}
		
		images.push_back( std::move(image) );
	}
	
	auto imagesAsPointers = std::vector<utilities::Image*>();
	for (auto& image : images)
	{
		imagesAsPointers.push_back( image.get() );
	}

	return addTexture2DArray( name, imagesAsPointers, settings );
}

Texture2DArray* TextureManager::addTexture2DArray(const std::string& name, const std::vector<utilities::Image*>& images, const TextureSettings settings)
{
	LOG_DEBUG( "Loading texture 2d array '" + name + "' from images." );

	auto it = textures2DArray_.find(name);

	if ( it != textures2DArray_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Texture 2d array already exists - returning already existing texture 2d array." );
		return it->second.get();
	}

	std::stringstream msg;
	msg << "TextureManager::addTexture2DArray: number of images: " << images.size();
	LOG_DEBUG( msg.str() );

	LOG_DEBUG( "Creating texture 2d array." );
	textures2DArray_[name] = std::unique_ptr<Texture2DArray>(new Texture2DArray(images, openGlDevice_, name, settings));

	return textures2DArray_[name].get();
}

}
}
