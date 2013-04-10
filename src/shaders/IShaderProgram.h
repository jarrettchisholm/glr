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

	virtual void bind() = 0;

	virtual GLuint getGLShaderProgramId() = 0;

	virtual IShader::BindingsMap getBindings() = 0;
	
	virtual std::string getName() = 0;
	
	virtual GLuint getBindPoint(std::string varName) = 0;
	virtual void bindData(GLuint, glm::vec2) = 0;
	virtual void bindData(GLuint, glm::vec3) = 0;
	virtual void bindData(GLuint, glm::vec4) = 0;
	virtual void bindData(GLuint, glm::mat2) = 0;
	virtual void bindData(GLuint, glm::mat3) = 0;
	virtual void bindData(GLuint, glm::mat4) = 0;
	virtual void bindData(GLuint, glmd::uint32) = 0;
	virtual void bindData(GLuint, glmd::float32) = 0;
	
	virtual void addBindListener(IShaderProgramBindListener* bindListener) = 0;
	virtual void removeBindListener(IShaderProgramBindListener* bindListener) = 0;
};
}
}
#endif /* ISHADERPROGRAM_H_ */
