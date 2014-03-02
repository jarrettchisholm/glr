#ifndef FUNCTIONBINDING_H_
#define FUNCTIONBINDING_H_

#include <string>

#include "Macros.hpp"

namespace glr
{
namespace cef_client
{

class FunctionBinding
{
public:
	FunctionBinding(std::wstring name);
	virtual ~FunctionBinding();
	
	// TODO: Get rid of macro setter and getter
	GETSET(std::wstring, name_, Name)

private:
	std::wstring name_;
};

}
}

#endif /* FUNCTIONBINDING_H_ */
