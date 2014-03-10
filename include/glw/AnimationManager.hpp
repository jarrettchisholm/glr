#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

#include <string>
#include <memory>
#include <map>

#include "IAnimationManager.hpp"

#include "IOpenGlDevice.hpp"

namespace glr
{
namespace glw
{

class Animation;

class AnimationManager : public IAnimationManager {
public:
	AnimationManager(IOpenGlDevice* openGlDevice);
	virtual ~AnimationManager();

	virtual Animation* getAnimation(const std::string& name) const;
	virtual Animation* addAnimation(const std::string& name);
	virtual Animation* addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes);
	
private:
	IOpenGlDevice* openGlDevice_;
	
	std::map< std::string, std::unique_ptr<Animation> > animations_;
};

}
}

#endif /* ANIMATIONMANAGER_H_ */
