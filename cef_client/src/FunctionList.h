/*
 * FunctionList.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <string>

#ifndef FUNCTIONLIST_H_
#define FUNCTIONLIST_H_

namespace glr {
namespace cef_client {
/*
enum Function
{
	ADD_FUNCTION = 0,
	REMOVE_FUNCTION,
	ADD_ATTRIBUTE,
	REMOVE_ATTRIBUTE,
	ADD_OBJECT,
	REMOVE_OBJECT,
	ADD_ATTRIBUTE_TO_OBJECT,
	REMOVE_ATTRIBUTE_FROM_OBJECT,
	ADD_METHOD_TO_OBJECT,
	REMOVE_METHOD_FROM_OBJECT,
	FUNCTION_RESULT,
	ALL_BINDINGS_SENT
};
*/

/**
 * Functions available to send to the browser:
 * 
 * ExecuteFunction
 * 		funcName [<argument> [, ...]]
 * ReadyForBindings
 *
 * AllBindingsReceived
 * 
 * 
 * Functions the browser can send us:
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
const std::wstring ADD_FUNCTION = L"AddFunction";
const std::wstring REMOVE_FUNCTION = L"RemoveFunction";
const std::wstring ADD_ATTRIBUTE = L"AddAttribute";
const std::wstring REMOVE_ATTRIBUTE = L"RemoveAttribute";
const std::wstring ADD_OBJECT = L"AddObject";
const std::wstring REMOVE_OBJECT = L"RemoveObject";
const std::wstring ADD_ATTRIBUTE_TO_OBJECT = L"AddAttributeToObject";
const std::wstring REMOVE_ATTRIBUTE_FROM_OBJECT = L"RemoveAttributeFromObject";
const std::wstring ADD_METHOD_TO_OBJECT = L"AddMethodToObject";
const std::wstring REMOVE_METHOD_FROM_OBJECT = L"RemoveMethodFromObject";
const std::wstring FUNCTION_RESULT = L"FunctionResult";
const std::wstring ALL_BINDINGS_SENT = L"AllBindingsSent";

// Functions to be sent to the browser process
const std::wstring EXECUTE_FUNCTION = L"ExecuteFunction";
const std::wstring READY_FOR_BINDINGS = L"ReadyForBindings";
const std::wstring ALL_BINDINGS_RECEIVED = L"AllBindingsReceived";

// Status values to be returned to a browser process
const std::wstring SUCCESS = L"Success";
const std::wstring EXCEPTION = L"Exception";

}
}

#endif /* FUNCTIONLIST_H_ */

