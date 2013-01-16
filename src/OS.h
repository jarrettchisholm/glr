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



class OS: public IOS {
public:
	OS();
	virtual ~OS();

	/**
	 *
	 */
	virtual glm::detail::int32 initialize() {
		return -1;
	}
	;

	/**
	 *
	 */
	virtual IWindow* createWindow(glm::detail::uint32 width = 800, glm::detail::uint32 height = 600, glm::detail::uint32 depth = 24,
			bool fullscreen = false, bool vsync = false) {
		return 0;
	}
	;

private:


};

}

#endif /* OS_H_ */
