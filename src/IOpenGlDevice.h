/*
 * IOpenGlDevice.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IOPENGLDEVICE_H_
#define IOPENGLDEVICE_H_

#include <string>

#include <GL/glew.h>

#include "glm/glm.hpp"

namespace glr {

struct GlError
{
	GLenum type = GL_NONE;
	std::string name = "";
};

class IOpenGlDevice {
public:
	virtual ~IOpenGlDevice()
	{
	}
	;

	virtual GLuint createBufferObject(GLenum target, glm::detail::uint32 totalSize, const void* dataPointer) = 0;
	virtual void releaseBufferObject(GLuint bufferId) = 0;
	virtual GLuint bindBuffer(GLuint bufferId) = 0;
	virtual void unbindBuffer(GLuint bufferId) = 0;
	
	virtual GlError getGlError() = 0;
};
}
#endif /* IOPENGLDEVICE_H_ */
