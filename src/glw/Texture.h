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
#include "../common/utilities/Macros.h"

namespace glr {
namespace glw {

namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture {
public:
	Texture(IOpenGlDevice* openGlDevice, const std::string name);
	Texture(utilities::Image* image, IOpenGlDevice* openGlDevice, const std::string name);
	virtual ~Texture();

	void bind(GLuint texturePosition = 0);
	
	GLuint getBufferId();
	GLuint getBindPoint();

	GETSET(std::string, name_, Name)

private:
	std::string name_;
	IOpenGlDevice* openGlDevice_;
	GLuint bufferId_;
	GLuint bindPoint_;
	
	//GLuint textureId_;

	void loadIntoVideoMemory(utilities::Image* image);
};

}
}
#endif /* TEXTURE_H_ */
