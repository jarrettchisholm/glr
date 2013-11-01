/*
 * BoostAnyUtilities.h
 *
 *  Created on: 2011-05-01
 *      Author: jarrett
 */

#ifndef BOOSTANYUTILITIES_H_
#define BOOSTANYUTILITIES_H_

#include <boost/any.hpp>

namespace utilities {

bool isEmpty(const boost::any& operand)
{
    return operand.empty();
}

bool isInt(const boost::any& operand)
{
    return operand.type() == typeid(int);
}

bool isUint(const boost::any& operand)
{
    return operand.type() == typeid(unsigned int);
}

bool isFloat(const boost::any& operand)
{
    return operand.type() == typeid(float);
}

bool isDouble(const boost::any& operand)
{
    return operand.type() == typeid(double);
}

bool isBool(const boost::any& operand)
{
    return operand.type() == typeid(bool);
}

/*
bool is_char_ptr(const boost::any & operand)
{
    try
    {
        boost::any_cast<const char *>(operand);
        return true;
    }
    catch(const boost::bad_any_cast &)
    {
        return false;
    }
}
*/

bool isString(const boost::any& operand)
{
    return boost::any_cast<std::string>(&operand);
}

bool isWstring(const boost::any& operand)
{
    return boost::any_cast<std::wstring>(&operand);
}

}
#endif /* BOOSTANYUTILITIES_H_ */












