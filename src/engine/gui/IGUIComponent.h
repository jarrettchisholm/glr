/*
 * IGUIComponent.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IGUICOMPONENT_H_
#define IGUICOMPONENT_H_

#include "glm/glm.hpp"

#include "IGUIObject.h"

namespace oglre {

namespace engine {

class IGUIComponent {
public:
	virtual ~IGUIComponent() {
	}
	;
	
	virtual int load() = 0;
	virtual void unload() = 0;
	
	virtual void executeScript(std::wstring script) = 0;
	
	virtual bool isVisible() = 0;
	virtual void setVisible(bool isVisible) = 0;
	
	/**
	 * Method createGUIObject
	 * 
	 * Create a GUIObject that your gui can use to talk to your application.
	 * 
	 * @param name The name associated with this GUIObject
	 * @return A pointer to an IGUIObject, or nullptr if the GUIObject could not be created successfully.
	 */
	virtual IGUIObject* createGUIObject(std::wstring name) = 0;
	
	virtual IGUIObject* getGUIObject(std::wstring name) = 0;
};

}

}

#endif /* IGUICOMPONENT_H_ */
