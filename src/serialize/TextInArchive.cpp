#include "serialize/TextInArchive.hpp"

namespace glr
{
namespace serialize
{

TextInArchive::TextInArchive(std::ifstream& ifstream) : boost::archive::text_iarchive(ifstream)
{
}

TextInArchive::~TextInArchive()
{
}

}
}
