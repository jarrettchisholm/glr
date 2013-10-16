/*
 * IGUI.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IADDFUNCTIONLISTENER_H_
#define IADDFUNCTIONLISTENER_H_

#include <string>

namespace glr {
namespace gui {
	
class IAddFunctionListener {
public:
	virtual ~IAddFunctionListener()
	{
	}
	;

	virtual void addedFunction(std::wstring func) = 0;
};

}
}
#endif /* IADDFUNCTIONLISTENER_H_ */
