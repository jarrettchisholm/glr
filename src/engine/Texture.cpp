/*
 * Texture.cpp
 *
 *  Created on: 2012-11-29
 *      Author: jarrett
 */

#include "Texture.h"


namespace icee {

namespace engine {

Texture::Texture(utilities::Image* image) {
	loadTexture(image);
}

Texture::~Texture() {
}

void Texture::loadTexture(utilities::Image* image) {
	glGenTextures(1, &textureId_);

	glBindTexture(GL_TEXTURE_2D, textureId_);
	// If the u,v coordinates overflow the range 0,1 the image is repeated
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // i don't combine the color with the original surface color, use only the texture map.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

    // And create 2d mipmaps for the minifying function
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
	// error check
	GLenum huboError = glGetError();
	if(huboError){
		BOOST_LOG_TRIVIAL(debug) << "Texture::loadTexture: error loading texture in opengl";
	}
}

}

}
