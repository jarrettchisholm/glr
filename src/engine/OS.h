/*
 * OS.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef OS_H_
#define OS_H_

#include "IOS.h"
#include "DefaultSceneManager.h"

namespace oglre {

namespace engine {

class OS: public IOS {
public:
	OS();
	virtual ~OS();

	/**
	 *
	 */
	virtual sint32 initialize() {
		return -1;
	}
	;

	/**
	 *
	 */
	virtual IWindow* createWindow(uint32 width = 800, uint32 height = 600, uint32 depth = 24,
			bool fullscreen = false, bool vsync = false) {
		return 0;
	}
	;

private:


};

}

}

#endif /* OS_H_ */
