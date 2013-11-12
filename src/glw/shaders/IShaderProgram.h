/*
 * IShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADERPROGRAM_H_
#define ISHADERPROGRAM_H_

#include <glm/glm.hpp>

#include <GL/glew.h>

#include "IShader.h"
#include "IShaderProgramBindListener.h"

namespace glr {
namespace shaders {

namespace glmd = glm::detail;

// Forward declaration due to circular dependency IShaderProgramBindListener
class IShaderProgramBindListener;
	
class IShaderProgram {
public:
	virtual ~IShaderProgram()
	{
	}
	;

	/**
	 * Binds this shader program as the currently active shader.  Any listeners attached to this shader program
	 * will be notified when this method is called.
	 */
	virtual void bind() = 0;

	virtual GLuint getGLShaderProgramId() = 0;

	/**
	 * Returns the bindings for this shader program.  Bindings are described in the IShader.h file.
	 * 
	 * @return The bindings for this shader program.
	 */
	virtual IShader::BindingsMap getBindings() = 0;
	
	virtual std::string getName() = 0;
	
	/**
	 * Bind the shader variable with name varName to the given bindPoint.
	 * 
	 * @param varName The shader variable name to bind.
	 * @param bindPoint The OpenGL bind point to bind the shader variable to.
	 */
	//virtual void bindVariable(std::string varName, GLuint bindPoint) = 0;
	/**
	 * Bind all shader variables of the type bindType to the given bindPoint.
	 * 
	 * @param bindType The shader variable type to bind.
	 * @param bindPoint The OpenGL bind point to bind the shader variable(s) to.
	 */
	//virtual void bindVariableByBindingName(IShader::BindType bindType, GLuint bindPoint) = 0;
	
	virtual GLuint getBindPointByVariableName(std::string varName) = 0;
	virtual GLuint getBindPointByBindingName(IShader::BindType bindType) = 0;
	
	/**
	 * Add a listener, which will be notified when this shader gets bound.
	 * 
	 * @param bindListener A pointer to an object implementing the IShaderProgramBindListener interface.
	 */
	virtual void addBindListener(IShaderProgramBindListener* bindListener) = 0;
	/**
	 * Remove a bind listener.
	 * 
	 * @param bindListener A pointer to an IShaderProgramBindListener object that should be removed as a bind listener for this shader.
	 */
	virtual void removeBindListener(IShaderProgramBindListener* bindListener) = 0;
};
}
}
#endif /* ISHADERPROGRAM_H_ */
