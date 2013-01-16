/*
 * IGUIObject.h
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#ifndef IGUIOBJECT_H_
#define IGUIOBJECT_H_

#include <functional>

#include "CallbackParameter.h"

namespace oglre {

namespace gui {

class IGUIObject {
public:
	virtual ~IGUIObject() {
	}
	;

	virtual void addFunction(std::wstring name, std::function<void()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<int()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<float()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<std::wstring()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<char()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<bool()> function) = 0;
	
	
	virtual void addFunction(std::wstring name, std::function<void(std::vector<CallbackParameter>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<int(std::vector<CallbackParameter>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<float(std::vector<CallbackParameter>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<std::wstring(std::vector<CallbackParameter>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<char(std::vector<CallbackParameter>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<bool(std::vector<CallbackParameter>)> function) = 0;
	
};

}

}

#endif /* IGUIOBJECT_H_ */
