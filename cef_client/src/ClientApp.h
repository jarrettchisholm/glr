/*
 * ClientApp.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <iostream>
#include <algorithm>
#include <mutex>

#include "ObjectBinding.h"
#include "FunctionList.h"
#include "ExceptionList.h"

#ifndef CLIENTAPP_H_
#define CLIENTAPP_H_

namespace glr {
namespace cef_client {

class ClientApp : public CefApp, public CefRenderProcessHandler, public CefV8Handler
{
public:
    ClientApp();
    
	virtual ~ClientApp();
	
	
	/** HELPER FUNCTIONS **/
	void copyToList(CefRefPtr<CefV8Value> value, int i, CefRefPtr<CefListValue> list);
	CefRefPtr<CefV8Value> getValue(CefRefPtr<CefProcessMessage> message, uint index);
	
	// Transfer a V8 array to a List.
	void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
	
	// Transfer a V8 value to a List index.
	void SetListValue(CefRefPtr<CefListValue> list, int index, CefRefPtr<CefV8Value> value);
	
	// Transfer a List value to a V8 array index.
	void SetListValue(CefRefPtr<CefV8Value> list, int index, CefRefPtr<CefListValue> value);
	
	// Transfer a List to a V8 array.
	void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);
	
	
	/**
	 * Implement CEF3's OnContextCreated method in order to add callable native functions to javascript.
	 */
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);	
	virtual void OnContextInitialized();	
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);	
	virtual void OnWebKitInitialized();
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler();
	
	/**
	 * Exceptions available:
	 * 
	 * BIND_EXCEPTION		- when something bad happens during binding
	 * EXECUTE_EXCEPTION	- when something bad happens during execution of a bound function
	 * MESSAGE_EXCEPTION	
	 */
	void sendMessageException(CefRefPtr<CefBrowser> browser, std::string messageId, Exception exception, std::wstring message);
	void sendMessageSuccess(CefRefPtr<CefBrowser> browser, std::string messageId, std::wstring message);
	void sendMessageAllBindingsReceived(CefRefPtr<CefBrowser> browser, uint numBindings);
	
	/**
	 * 
	 * 
	 * List of available functions:
	 * 
	 * AddFunction
	 * 		funcName
	 * RemoveFunction
	 * 		funcName
	 * AddAttribute
	 * 		attrName
	 * RemoveAttribute
	 * 		attrName
	 * AddObject
	 * 		objName
	 * RemoveObject
	 * 		objName
	 * AddAttributeToObject
	 * 		objName attrName
	 * RemoveAttributeFromObject
	 * 		objName attrName
	 * AddMethodToObject
	 * 		objName methodName
	 * RemoveMethodFromObject
	 * 		objName methodName
	 * FunctionResult
	 * 		resultType resultValue
	 * AllBindingsSent
	 * 
	 */
	virtual bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
	
	/*
	 * When your javascript calls a method that has been defined as a native api call, it will call this method.
	 * 
	 * This method only binds one function - namely, the setMessageCallback method.  It is the method that we call
	 * once we have a return result from a native function call.
	 */
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception);
	
private:
	// Map of message callbacks.
	typedef std::map<std::pair<std::wstring, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > > CallbackMap;
	CallbackMap callbackMap_;
	
	// Object list
	std::map < std::wstring, ObjectBinding > objectBindingMap_;
	std::mutex objectBindingMapMutex_;
	
	int totalBindingsSent_;
	int totalBindingsReceived_;
	bool allBindingsSentMessageReceived_;
	bool readyForBindingsMessageSent_;
	

	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(ClientApp)	
};

}
}

#endif /* CLIENTAPP_H_ */
