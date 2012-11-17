/*
 * HtmlGuiComponent.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef HTMLGUICOMPONENT_H_
#define HTMLGUICOMPONENT_H_

#include <string>

#include "IGUIComponent.h"

namespace icee {

namespace engine {

class HtmlGuiComponent : public IGUIComponent {
public:
	HtmlGuiComponent();
	virtual ~HtmlGuiComponent();
	
	virtual int initialize();
	virtual void destroy();
	
	int setContents(std::string contents);
	int loadContentsFromFile(std::string filename);
	
	void render();
	
	virtual void setVisible(bool isVisible);
};

}

}

#endif /* HTMLGUICOMPONENT_H_ */
