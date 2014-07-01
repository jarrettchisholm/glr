#ifndef EXCEPTIONLIST_H_
#define EXCEPTIONLIST_H_

#include <map>
#include <string>

namespace glr
{
namespace cef_client
{

/**
 * Exceptions available:
 * 
 * BIND_EXCEPTION		- when something bad happens during binding
 * JS_BIND_EXCEPTION	- when something bad happens during binding of a js callback function
 * EXECUTE_EXCEPTION	- when something bad happens during execution of a bound function
 * MESSAGE_EXCEPTION	
 */
enum Exception
{
	BIND_EXCEPTION = 0,
	JS_BIND_EXCEPTION,
	EXECUTE_EXCEPTION,
	MESSAGE_EXCEPTION
};

namespace
{

std::string getStringName(Exception exception)
{
	switch (exception)
	{
		case Exception::BIND_EXCEPTION:
			return std::string("BindException");
			break;
		
		case Exception::JS_BIND_EXCEPTION:
			return std::string("JsBindException");
			break;
		
		case Exception::EXECUTE_EXCEPTION:
			return std::string("ExecuteException");
			break;
		
		case Exception::MESSAGE_EXCEPTION:
			return std::string("MessageException");
			break;
		
		default:
			return std::string();
	}
	
	return std::string();
}

}

}
}

#endif /* EXCEPTIONLIST_H_ */

