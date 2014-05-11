#include <utility>

#include "exceptions/IoException.hpp"

namespace glr
{
namespace exception
{

IoException::IoException(std::string message) : Exception(std::move(message))
{
}

IoException::~IoException() throw()
{
}

}
}
