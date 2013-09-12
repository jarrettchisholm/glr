/*
 * Texture.cpp
 *
 *  Created on: 2012-11-29
 *      Author: jarrett
 */

#include "Texture.h"


namespace glr {
namespace glw {
	
Texture::Texture(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

Texture::Texture(utilities::Image* image, IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
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

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB,  image->width, image->height, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
	
	//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, image->width, image->height, 1, GL_RGB, GL_UNSIGNED_BYTE, image->data);



	// error check
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// TODO: throw error
		BOOST_LOG_TRIVIAL(error) << "Texture::loadIntoVideoMemory: error loading texture in opengl";
		BOOST_LOG_TRIVIAL(error) << "OpenGL error: " << err.name;
	}
	else
	{
		BOOST_LOG_TRIVIAL(debug) << "Successfully loaded texture.";
	}
}

void Texture::bind(GLuint texturePosition)
{
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, bufferId_);
	
	bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );

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
