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
	
	virtual void render() = 0;
	virtual IGUIComponent* load(std::string filename) = 0;
};

}

}

#endif /* IGUI_H_ */
