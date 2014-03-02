#include "DuplicateAttributeException.hpp"

namespace glr
{
namespace cef_client
{
	
DuplicateAttributeException::DuplicateAttributeException(std::wstring attrName) : std::runtime_error("Attribute with name already exists.")
{
}

DuplicateAttributeException::~DuplicateAttributeException() throw()
{
}

}
}
