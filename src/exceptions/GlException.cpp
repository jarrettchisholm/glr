#include <utility>

#include "exceptions/GlException.hpp"

namespace glr
{
namespace exception
{

GlException::GlException(std::string message) : Exception(std::move(message))
{
}

GlException::~GlException() throw()
{
}

}
}
