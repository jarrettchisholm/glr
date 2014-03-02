#include "DuplicateFunctionException.hpp"

namespace glr
{
namespace cef_client
{
	
DuplicateFunctionException::DuplicateFunctionException(std::wstring funcName) : std::runtime_error("Function with name already exists.")
{
}

DuplicateFunctionException::~DuplicateFunctionException() throw()
{
}

}
}
