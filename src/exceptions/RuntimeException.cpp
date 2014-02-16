#include "RuntimeException.h"

namespace glr {
namespace exception {

RuntimeException::RuntimeException(const std::string message) : Exception(message)
{
}

RuntimeException::~RuntimeException() throw()
{
}

}
}
