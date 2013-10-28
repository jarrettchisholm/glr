/*
 * FunctionBinding.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */
#include <string>

#include "Macros.h"

#ifndef FUNCTIONBINDING_H_
#define FUNCTIONBINDING_H_

namespace glr {
namespace cef_client {

class FunctionBinding
{
public:
	FunctionBinding(std::wstring name);
	virtual ~FunctionBinding();
	
	GETSET(std::wstring, name_, Name)

private:
	std::wstring name_;
};

}
}

#endif /* FUNCTIONBINDING_H_ */
