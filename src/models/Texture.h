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

#include "../IMatrixData.h"
#include "../shaders/IShaderProgram.h"

#include "../common/utilities/ImageLoader.h"

namespace glr {
namespace models {
namespace glmd = glm::detail;

class Texture {
public:
	Texture();
	Texture(utilities::Image* image);
	virtual ~Texture();

	void bind(IMatrixData* matrixData, shaders::IShaderProgram* shader);

private:
	GLuint textureId_;

	void loadTexture(utilities::Image* image);
};
}
}
#endif /* TEXTURE_H_ */
