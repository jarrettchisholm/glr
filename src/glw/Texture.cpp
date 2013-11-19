/*
 * Texture.cpp
 *
 *  Created on: 2012-11-29
 *      Author: jarrett
 */

#include <sstream>

#include "Texture.h"

#include "../exceptions/GlException.h"
#include "../exceptions/FormatException.h"


namespace glr {
namespace glw {
	
Texture::Texture(IOpenGlDevice* openGlDevice, const std::string name) : openGlDevice_(openGlDevice), name_(name)
{
}

Texture::Texture(utilities::Image* image, IOpenGlDevice* openGlDevice, const std::string name) : openGlDevice_(openGlDevice), name_(name)
{
	if ( image == nullptr )
		bufferId_ = 0;
	else
		loadIntoVideoMemory(image);
}

Texture::~Texture()
{
}

void Texture::loadIntoVideoMemory(utilities::Image* image)
{
	glGenTextures(1, &bufferId_);

	glBindTexture(GL_TEXTURE_2D_ARRAY, bufferId_);
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	GLint internalFormat = GL_RGB;
	
	switch (image->format)
	{
		case utilities::Format::FORMAT_RGB:
			// Assigned as the default value
			break;
		
		case utilities::Format::FORMAT_RGBA:
			internalFormat = GL_RGBA;
			break;
		
		default:
		{
			std::string msg = std::string( "Texture::loadIntoVideoMemory: Unknown image format." );
			LOG_ERROR( msg );
			throw exception::FormatException( msg );
		}
	}
	
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat,  image->width, image->height, 1, 0, internalFormat, GL_UNSIGNED_BYTE, image->data);

	// error check
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// Cleanup
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glDeleteTextures(1, &bufferId_);
		
		std::string msg = std::string( "Error while loading texture '" + name_ + "' in OpenGL: " + err.name);
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully loaded texture." );
	}
}

void Texture::bind(GLuint texturePosition)
{	
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, bufferId_);
	
	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
	//std::cout << "texture: " << name_ << " | " << bufferId_ << " | " << bindPoint_ << std::endl;
	// to unbind, we use the following
	 //glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

GLuint Texture::getBufferId()
{
	return bufferId_;
}

GLuint Texture::getBindPoint()
{
	return bindPoint_;
}

}
}
