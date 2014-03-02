#ifndef EXCEPTIONLIST_H_
#define EXCEPTIONLIST_H_

namespace glr
{
namespace cef_client
{

/**
 * Exceptions available:
 * 
 * BIND_EXCEPTION		- when something bad happens during binding
 * EXECUTE_EXCEPTION	- when something bad happens during execution of a bound function
 * MESSAGE_EXCEPTION	
 */
enum Exception
{
	BIND_EXCEPTION = 0,
	EXECUTE_EXCEPTION,
	MESSAGE_EXCEPTION
};

}
}

#endif /* EXCEPTIONLIST_H_ */

