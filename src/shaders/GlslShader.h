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

namespace glr {
namespace shaders {
class GlslShader : public IShader {
public:
	typedef std::vector< std::pair<std::string, std::string> > StringBindingsMap;

	GlslShader(std::string source, Type type);
	GlslShader(std::string name, std::string source, Type type, StringBindingsMap bindings);
	virtual ~GlslShader();

	virtual void bind();
	virtual IShader::Type getType();
	virtual GLuint getGLShaderId();
	virtual IShader::BindingsMap getBindings();

	void compile();

	std::string getName();

private:
	GLuint shaderId_;

	std::string name_;
	std::string source_;
	Type type_;

	BindingsMap bindings_;
};
}
}
#endif /* GLSLSHADER_H_ */
