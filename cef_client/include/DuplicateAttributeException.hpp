#ifndef DUPLICATEATTRIBUTEEXCEPTION_H_
#define DUPLICATEATTRIBUTEEXCEPTION_H_

#include <stdexcept>

namespace glr
{
namespace cef_client
{

class DuplicateAttributeException : public std::runtime_error
{
public:
	DuplicateAttributeException(std::wstring attrName);
	virtual ~DuplicateAttributeException() throw();
};

}
}

#endif /* DUPLICATEATTRIBUTEEXCEPTION_H_ */
