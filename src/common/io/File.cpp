/*
 * File.cpp
 *
 *  Created on: 2010-12-23
 *      Author: jarrett
 */

#include "File.h"
#include <string.h>

#include <string>
#include <fstream>
#include <streambuf>

namespace glr {
namespace io {

std::string File::getFileContents(const std::string filename)
{
	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());

	return str;
}

}
}
