/*
 * Shader.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <glm/glm.hpp>

#include "IShader.h"

namespace oglre {

namespace shaders {

class Shader : public IShader {
public:
	Shader(std::string sourceCode, Type type);
	virtual ~Shader();
	
	virtual void bind();
	virtual IShader::Type getType();
	virtual GLuint getGLShaderId();
	
	static IShader::Type parseType(std::string type);

private:
	GLuint shaderId_;
	
	Type type_;
	
	std::string sourceCode_;
	
	void initialize();
};

}

}

#endif /* SHADER_H_ */
