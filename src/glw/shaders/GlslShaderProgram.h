/*
 * GlslShaderProgram.h
 * 
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef GLSLSHADERPROGRAM_H_
#define GLSLSHADERPROGRAM_H_

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "IShaderProgram.h"

#include "GlslShader.h"

namespace glr {
namespace shaders {

/**
 * Class representing a 'glsl' shader program.  This is a shader program that holds all of the processed shader source required for this shader program.
 */
class GlslShaderProgram : public IShaderProgram {
public:
	GlslShaderProgram(std::string name, std::vector< std::shared_ptr<GlslShader> > shaders);
	virtual ~GlslShaderProgram();

	virtual void bind();
	virtual GLuint getGLShaderProgramId();
	virtual IShader::BindingsMap getBindings();
	void compile();

	virtual void bindVariable(std::string varName, GLuint bindPoint);
	virtual void bindVariableByBindingName(IShader::BindType bindType, GLuint bindPoint);

	std::string getName();
	
	virtual void addBindListener(IShaderProgramBindListener* bindListener);
	virtual void removeBindListener(IShaderProgramBindListener* bindListener);
	void removeAllBindListeners();

	static void unbindAll();

private:
	GLuint programId_;

	std::string name_;

	std::vector< std::shared_ptr<GlslShader> > shaders_;
	IShader::BindingsMap bindings_;
	
	std::vector<IShaderProgramBindListener*> bindListeners_;
	
	void generateBindings();
};

}
}
#endif /* GLSLSHADERPROGRAM_H_ */
