#include "exceptions/GlException.hpp"

namespace glr
{
namespace exception
{

GlException::GlException(const std::string message) : Exception(message)
{
}

GlException::~GlException() throw()
{
}

}
}
