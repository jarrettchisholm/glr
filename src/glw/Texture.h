/*
 * Texture.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/IShaderProgram.h"
#include "IOpenGlDevice.h"

#include "../common/utilities/ImageLoader.h"

namespace glr {
namespace glw {
namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture {
public:
	Texture(IOpenGlDevice* openGlDevice);
	Texture(utilities::Image* image, IOpenGlDevice* openGlDevice);
	virtual ~Texture();

	void bind(GLuint texturePosition = 0);
	
	GLuint getBufferId();
	GLuint getBindPoint();

private:	
	IOpenGlDevice* openGlDevice_;
	GLuint bufferId_;
	GLuint bindPoint_;
	
	//GLuint textureId_;

	void loadIntoVideoMemory(utilities::Image* image);
};
}
}
#endif /* TEXTURE_H_ */
