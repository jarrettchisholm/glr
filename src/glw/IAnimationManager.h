#ifndef IANIMATIONMANAGER_H_
#define IANIMATIONMANAGER_H_

#include <string>

#include "Animation.h"

namespace glr
{
namespace glw
{

class Animation;

class IAnimationManager {
public:
	virtual ~IAnimationManager()
	{
	}
	;

	virtual Animation* getAnimation(const std::string& name) = 0;
	virtual Animation* addAnimation(const std::string& name) = 0;
	virtual Animation* addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes) = 0;
};

}
}
#endif /* IANIMATIONMANAGER_H_ */
