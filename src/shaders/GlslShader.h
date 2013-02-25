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
	GlslShader(std::string source, Type type);
	GlslShader(std::string name, std::string source, Type type, BindingsMap bindings);
	virtual ~GlslShader();
	
	virtual void bind();
	virtual IShader::Type getType();
	virtual GLuint getGLShaderId();
	
	void compile();
	
	std::string getName();
	GlslShader::BindingsMap getBindings();

private:
	GLuint shaderId_;
	
	Type type_;
	std::string name_;
	
	std::string source_;
	
	BindingsMap bindings_;
};

}

}

#endif /* GLSLSHADER_H_ */
