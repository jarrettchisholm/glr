/*
 * IGUIComponent.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IGUICOMPONENT_H_
#define IGUICOMPONENT_H_

namespace icee {

namespace engine {

class IGUIComponent {
public:
	virtual ~IGUIComponent() {
	}
	;
	
	virtual int initialize() = 0;
	virtual void destroy() = 0;
	
	virtual void setVisible(bool isVisible) = 0;
};

}

}

#endif /* IGUICOMPONENT_H_ */
