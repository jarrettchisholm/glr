/*
 * IOS.h
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

#ifndef IOS_H_
#define IOS_H_

#include "IWindow.h"
#include "ISceneManager.h"

#include "../common/compatibility/Types.h"

namespace oglre {

namespace engine {

using namespace compatibility;

class IOS {
public:
	virtual ~IOS() {
	}
	;

	/**
	 *
	 */
	virtual sint32 initialize() = 0;

	/**
	 *
	 */
	virtual IWindow* createWindow(uint32 width = 800, uint32 height = 600, uint32 depth = 24,
			bool fullscreen = false, bool vsync = false) = 0;
};

}

}

#endif /* IOS_H_ */
