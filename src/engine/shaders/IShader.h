/*
 * IShader.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADER_H_
#define ISHADER_H_

#include <GL/glew.h>

namespace oglre {

namespace shaders {

class IShader {
public:
	virtual ~IShader() {
	}
	;
	
	virtual void bind() = 0;
	virtual GLuint getGLShaderId() = 0;
};

}

}

#endif /* ISHADER_H_ */
