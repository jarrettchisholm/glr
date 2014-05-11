#include "exceptions/IoException.hpp"

namespace glr
{
namespace exception
{

IoException::IoException(const std::string& message) : Exception(message)
{
}

IoException::~IoException() throw()
{
}

}
}
