/*
 * Shader.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <glm/glm.hpp>

#include "IShader.h"

namespace oglre {

namespace shaders {

class Shader : public IShader {
public:
	Shader();
	virtual ~Shader();
	
	virtual void bind();
	virtual GLuint getGLShaderId();
	
	glm::detail::int32 initialize();

private:
	GLuint shaderId_;

	void loadShader();
};

}

}

#endif /* SHADER_H_ */
