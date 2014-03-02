#include <string.h>

#include <string>
#include <fstream>
#include <streambuf>

#include "File.hpp"

namespace glr
{
namespace io
{

std::string getFileContents(const std::string& filename)
{
	std::ifstream stream(filename);
	std::string str = std::string( (std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>() );

	return str;
}

}
}
