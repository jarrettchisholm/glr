/*
 * GlslShader.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

#include <string>

#include <glm/glm.hpp>

#include "IShader.h"

namespace oglre {

namespace shaders {

class GlslShader : public IShader {
public:
	GlslShader(std::string sourceCode, Type type);
	virtual ~GlslShader();
	
	virtual void bind();
	virtual IShader::Type getType();
	virtual GLuint getGLGlslShaderId();
	
	void compile();
	
	std::string getName();
	
	static IShader::Type parseType(std::string type);

private:
	GLuint shaderId_;
	
	Type type_;
	std::string name_;
	
	std::string sourceCode_;
};

}

}

#endif /* GLSLSHADER_H_ */
