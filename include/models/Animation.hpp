#ifndef MODELANIMATION_H_
#define MODELANIMATION_H_

#include <memory>
#include <map>
#include <string>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IAnimation.hpp"
#include "glw/IOpenGlDevice.hpp"

#include "glw/Animation.hpp"

namespace glmd = glm::detail;

namespace glr
{
namespace models
{


/**
 * Animation class.  This class is essentially a wrapper around the glw::Animation object.
 * 
 * It does this so that a Model object can be used with a 'unique' set of Animation objects.  Each
 * Animation object can then have it's own animation running time.
 * 
 * This ultimately means we can have several models that have the 'same' animations, and they
 * will be able to run through the animations at different speeds (as each animation will have
 * its own animation running time).
 */	
class Animation : public IAnimation
{
public:
	Animation(glw::Animation* animation, glw::IOpenGlDevice* openGlDevice);
	Animation(const Animation& other);
	virtual ~Animation();
	
	glw::Animation* getAnimation();
	glm::detail::float32 getAnimationTime();
	
	glm::detail::uint32 getStartFrame();
	glm::detail::uint32 getEndFrame();
	
	// From glw::IAnimation
	virtual const std::string& getName();
	virtual void setAnimationTime(glm::detail::float32 runningTime);
	
	virtual void setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame); 
	
	std::vector<glmd::uint32>& getIndexCache();

protected:
	glw::IOpenGlDevice* openGlDevice_;
	
	glw::Animation* animation_;
	
	// TESTING
	std::vector<glmd::uint32> indexCache_;
	
	glm::detail::float32 runningTime_;
	
	// Only play frames within this range
	glmd::uint32 startFrame_;
	glmd::uint32 endFrame_;
	
private:
	void initialize(glw::Animation* animation);
	void destroy();

};

}
}

#endif /* MODELANIMATION_H_ */
