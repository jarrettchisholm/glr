#ifndef ATTRIBUTEBINDING_H_
#define ATTRIBUTEBINDING_H_

#include <string>

#include <cef_app.h>

#include "Macros.hpp"

namespace glr
{
namespace cef_client
{

class AttributeBinding
{
public:
	AttributeBinding(std::wstring name);
	AttributeBinding(std::wstring name, std::wstring valueWstring);
	AttributeBinding(std::wstring name, std::string valueString);
	AttributeBinding(std::wstring name, int valueInt);
	AttributeBinding(std::wstring name, unsigned int valueUint);
	AttributeBinding(std::wstring name, bool valueBool);
	AttributeBinding(std::wstring name, float valueFloat);
	AttributeBinding(std::wstring name, double valueDouble);
	AttributeBinding(std::wstring name, CefRefPtr<CefV8Value> valueV8);
	
	virtual ~AttributeBinding();

	// TODO: Get rid of macros for getters and setters
	GETSET(std::wstring, name_, Name)
	
	GETSET(std::wstring, valueWstring_, ValueWstring)
	GETSET(std::string, valueString_, ValueString)
	GETSET(int, valueInt_, ValueInt)
	GETSET(unsigned int, valueUint_, ValueUint)
	GETSET(bool, valueBool_, ValueBool)
	GETSET(float, valueFloat_, ValueFloat)
	GETSET(double, valueDouble_, ValueDouble)
	GETSET(CefRefPtr<CefV8Value>, valueV8_, ValueV8)
	
	GETSET(bool, isWstring_, IsWstring)
	GETSET(bool, isString_, IsString)
	GETSET(bool, isInt_, IsInt)
	GETSET(bool, isUint_, IsUint)
	GETSET(bool, isBool_, IsBool)
	GETSET(bool, isFloat_, IsFloat)
	GETSET(bool, isDouble_, IsDouble)
	GETSET(bool, isV8_, IsV8)

private:
	std::wstring name_;
	
	std::wstring valueWstring_;
	std::string valueString_;
	int valueInt_;
	unsigned int valueUint_;
	bool valueBool_;
	float valueFloat_;
	double valueDouble_;
	CefRefPtr<CefV8Value> valueV8_;
	
	bool isWstring_;
	bool isString_;
	bool isInt_;
	bool isUint_;
	bool isBool_;
	bool isFloat_;
	bool isDouble_;
	bool isV8_;
	
};

}
}

#endif /* ATTRIBUTEBINDING_H_ */
