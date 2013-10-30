/*
 * StringUtilities.h
 *
 *  Created on: 2011-05-01
 *      Author: jarrett
 */

#ifndef STRINGUTILITIES_H_
#define STRINGUTILITIES_H_

#include <string>

namespace utilities {

std::string toString(const std::wstring ws)
{
	return std::string( ws.begin(), ws.end() );
}

std::wstring toWstring(const std::string s)
{
	return std::wstring( s.begin(), s.end() );
}

}
#endif /* STRINGUTILITIES_H_ */













