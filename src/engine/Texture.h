/*
 * Texture.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/gl.h>

#include "../common/utilities/ImageLoader.h"

#include "../common/compatibility/Types.h"

namespace icee {

namespace engine {

using namespace compatibility;

class Texture {
public:
	Texture();
	Texture(utilities::Image* image);
	virtual ~Texture();

private:
	GLuint textureId_;

	void loadTexture(utilities::Image* image);
};

}

}

#endif /* TEXTURE_H_ */
