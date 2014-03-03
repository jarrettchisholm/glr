#ifndef IMODELANIMATION_H_
#define IMODELANIMATION_H_

#define GLM_FORCE_RADIANS
#include "glm/gtc/quaternion.hpp"

#include "glw/shaders/IShaderProgram.hpp"

#include "glw/IAnimation.hpp"

namespace glr
{
namespace models
{

/**
 * Class to wrap around the glw::IAnimation class.
 */
class IAnimation  : public glw::IAnimation
{
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
