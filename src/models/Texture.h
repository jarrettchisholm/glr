/*
 * Texture.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>

#include "../common/utilities/ImageLoader.h"

namespace glr {

namespace models {

class Texture {
public:
	Texture();
	Texture(utilities::Image* image);
	virtual ~Texture();
	
	void bind();

private:
	GLuint textureId_;

	void loadTexture(utilities::Image* image);
};

}

}

#endif /* TEXTURE_H_ */
