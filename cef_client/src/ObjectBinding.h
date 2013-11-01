/*
 * ObjectBinding.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "FunctionBinding.h"
#include "AttributeBinding.h"

#include "ExceptionList.h"

#ifndef OBJECTBINDING_H_
#define OBJECTBINDING_H_

namespace glr {
namespace cef_client {

class ObjectBinding : public CefV8Handler
{
public:
	ObjectBinding();
	ObjectBinding(std::wstring name);
	
	virtual ~ObjectBinding();
	
	/*
	 * When your javascript calls a method that has been defined as a native api call, it will call this method.
	 * 
	 * This method will encode the function call, and send it to the browser process over IPC.
	 * 
	 * The format that is sent via IPC looks like this:
	 * <function_name> <number_of_arguments> [<argument>, ..]
	 * 
	 * The browser process will process the function call, execute the requested function (if it exists), and send the result via IPC to this
	 * process.
	 */
	bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception);

	bool hasFunction(std::wstring funcName);
	bool hasAttribute(std::wstring attrName);
	void addFunction(FunctionBinding function);
	void addAttribute(AttributeBinding attribute);

	const std::vector< FunctionBinding > getFunctions();
	const std::vector< AttributeBinding > getAttributes();
	
	GETSET(std::wstring, name_, Name)
	
private:
	std::wstring name_;
	std::vector< FunctionBinding > functions_;
	std::vector< AttributeBinding > attributes_;

	void sendMessageException(CefRefPtr<CefBrowser> browser, std::string messageId, Exception exception, std::wstring message);

	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING( ObjectBinding )
};

}
}

#endif /* OBJECTBINDING_H_ */
