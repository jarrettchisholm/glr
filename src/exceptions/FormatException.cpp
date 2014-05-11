#include "exceptions/FormatException.hpp"

namespace glr
{
namespace exception
{

FormatException::FormatException(const std::string& message) : Exception(message)
{
}

FormatException::~FormatException() throw()
{
}

}
}
