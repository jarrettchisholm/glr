#include "serialize/TextOutArchive.hpp"

namespace glr
{
namespace serialize
{

TextOutArchive::TextOutArchive(std::ofstream& ofstream) : boost::archive::text_oarchive(ofstream)
{
}

TextOutArchive::~TextOutArchive()
{
}

}
}
