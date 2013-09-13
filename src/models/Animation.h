/*
 * Animation.h
 *
 *  Created on: 2012-09-20
 *      Author: jarrett
 */

#ifndef MODELANIMATION_H_
#define MODELANIMATION_H_

#include <memory>
#include <map>
#include <string>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IAnimation.h"
#include "../glw/IOpenGlDevice.h"

#include "../glw/Animation.h"

namespace glr {
namespace models {
	

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
class Animation : public IAnimation {
public:
	Animation(glw::Animation* animation, glw::IOpenGlDevice* openGlDevice);
	Animation(const Animation& other);
	virtual ~Animation();
	
	glw::Animation* getAnimation();
	glm::detail::float32 getAnimationTime();
	
	// From glw::IAnimation
	virtual const std::string getName();
	virtual void setAnimationTime(glm::detail::float32 runningTime);

protected:
	glw::IOpenGlDevice* openGlDevice_;
	
	glw::Animation* animation_;
	
	glm::detail::float32 runningTime_;
	
private:
	void initialize(glw::Animation* animation);
	void destroy();

};
}
}
#endif /* MODELANIMATION_H_ */
