/*
 * IOpenGlDevice.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IOPENGLDEVICE_H_
#define IOPENGLDEVICE_H_

#include <GL/glew.h>

#include "glm/glm.hpp"

namespace glr {

class IOpenGlDevice {
public:
	virtual ~IOpenGlDevice()
	{
	}
	;

	virtual GLuint createBufferObject(glm::detail::uint32 totalSize, void* dataPointer) = 0;
	virtual void releaseBufferObject(GLuint bufferId) = 0;
	virtual GLuint bindBuffer(GLuint bufferId) = 0;
	virtual GLuint unbindBuffer(GLuint bufferId) = 0;
};
}
#endif /* IOPENGLDEVICE_H_ */
