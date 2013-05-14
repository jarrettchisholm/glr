/*
 * IAnimationManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IANIMATIONMANAGER_H_
#define IANIMATIONMANAGER_H_

#include <string>

#include "Animation.h"

namespace glr {
namespace glw {

class Animation;

class IAnimationManager {
public:
	virtual ~IAnimationManager()
	{
	}
	;

	virtual Animation* getAnimation(const std::string path) = 0;
	virtual Animation* addAnimation(const std::string path) = 0;
};

}
}
#endif /* IANIMATIONMANAGER_H_ */
