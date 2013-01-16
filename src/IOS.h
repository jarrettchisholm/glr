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

namespace oglre {

class IOS {
public:
	virtual ~IOS() {
	}
	;

	/**
	 *
	 */
	virtual glm::detail::int32 initialize() = 0;

	/**
	 *
	 */
	virtual IWindow* createWindow(glm::detail::uint32 width = 800, glm::detail::uint32 height = 600, glm::detail::uint32 depth = 24,
			bool fullscreen = false, bool vsync = false) = 0;
};

}

#endif /* IOS_H_ */
