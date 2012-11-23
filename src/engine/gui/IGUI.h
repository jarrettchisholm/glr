/*
 * IGUI.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IGUI_H_
#define IGUI_H_

#include <string>

#include "IGUIComponent.h"

namespace icee {

namespace engine {

class IGUI {
public:
	virtual ~IGUI() {
	}
	;
	
	virtual int initialize() = 0;
	virtual void destroy() = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	virtual IGUIComponent* loadFromFile(std::string filename) = 0;
	virtual IGUIComponent* loadFromData(std::string data) = 0;
	virtual int release(IGUIComponent*) = 0;
};

}

}

#endif /* IGUI_H_ */
