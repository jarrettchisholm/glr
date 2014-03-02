#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

namespace glr
{
namespace io
{

static std::string getFileContents(const std::string& filename);

}
}

#endif /* FILE_H_ */
