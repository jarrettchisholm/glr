#ifndef GLEXCEPTION_H_
#define GLEXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace glr
{
namespace exception
{

class GlException : public Exception
{
public:
	GlException(const std::string& message = std::string("Glr GlException"));
	virtual ~GlException() throw();
};

}
}

#endif /* GLEXCEPTION_H_ */
