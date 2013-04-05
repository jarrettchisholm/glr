/*
 * GlrShaderProgram.h
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */


#ifndef GLRSHADERPROGRAM_H
#define GLRSHADERPROGRAM_H

#include <memory>

#include "GlrShader.h"

namespace glr {
namespace shaders {
class GlrShaderProgram {
public:
	GlrShaderProgram(std::string source);
	virtual ~GlrShaderProgram();

	void process(std::map< std::string, std::shared_ptr<GlrShader> > glrShaderMap);

	std::string getName();

	std::vector< std::shared_ptr<GlrShader> > getShaders();

private:
	std::string name_;
	std::string source_;

	std::vector< std::shared_ptr<GlrShader> > shaders_;
};
}
}
#endif /* GLRSHADERPROGRAM_H */
