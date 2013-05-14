/*
 * AnimationManager.h
 *
 *  Created on: 2013-05-13
 *      Author: jarrett
 */

#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

#include <string>
#include <memory>
#include <map>

#include "IAnimationManager.h"

#include "IOpenGlDevice.h"

namespace glr {
namespace glw {

class Animation;

class AnimationManager : public IAnimationManager {
public:
	AnimationManager(IOpenGlDevice* openGlDevice);
	virtual ~AnimationManager();

	virtual Animation* getAnimation(const std::string path);
	virtual Animation* addAnimation(const std::string path);
	
private:
	IOpenGlDevice* openGlDevice_;
	
	std::map< std::string, std::unique_ptr<Animation> > animations_;
};

}
}
#endif /* ANIMATIONMANAGER_H_ */
