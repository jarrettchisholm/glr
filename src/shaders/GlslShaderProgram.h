/*
 * GlslShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef GLSLSHADERPROGRAM_H_
#define GLSLSHADERPROGRAM_H_

#include <vector>

#include <glm/glm.hpp>

#include "IShaderProgram.h"

#include "GlslShader.h"

namespace oglre {

namespace shaders {

class GlslShaderProgram : public IShaderProgram {	
public:
	GlslShaderProgram(std::string name, std::vector< std::shared_ptr<GlslShader> > shaders);
	virtual ~GlslShaderProgram();
	
	virtual void bind();
	virtual GLuint getGLShaderProgramId();
	virtual IShader::BindingsMap getBindings();
	void compile();
	
	
	std::string getName();
	
	static void unbindAll();

private:
	GLuint programId_;
	
	std::string name_;

	std::vector< std::shared_ptr<GlslShader> > shaders_;
};

}

}

#endif /* GLSLSHADERPROGRAM_H_ */
