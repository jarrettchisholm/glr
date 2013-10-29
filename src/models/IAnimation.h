/*
 * IAnimation.h
 *
 *  Created on: 2012-10-20
 *      Author: jarrett
 */

#ifndef IMODELANIMATION_H_
#define IMODELANIMATION_H_

#include "glm/gtc/quaternion.hpp"

#include "../glw/shaders/IShaderProgram.h"

#include "../glw/IAnimation.h"

namespace glr {
namespace models {

/**
 * Class to wrap around the glw::IAnimation class.
 */
class IAnimation  : public glw::IAnimation {
public:
	virtual ~IAnimation()
	{
	}
	;
	
	virtual void setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame) = 0; 

	
};
}
}
#endif /* IMODELANIMATION_H_ */
