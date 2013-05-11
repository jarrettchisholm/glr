/*
 * IShaderProgramBindListener.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADERPROGRAMBINDLISTENER_H_
#define ISHADERPROGRAMBINDLISTENER_H_

#include "IShaderProgram.h"

namespace glr {
namespace shaders {
// Forward declaration due to circular dependency IShaderProgram
class IShaderProgram;

class IShaderProgramBindListener {
public:
	virtual ~IShaderProgramBindListener()
	{
	}
	;

	virtual void shaderBindCallback(IShaderProgram* shader) = 0;
};
}
}
#endif /* ISHADERPROGRAMBINDLISTENER_H_ */
