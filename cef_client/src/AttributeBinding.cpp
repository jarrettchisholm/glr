#include "AttributeBinding.h"

namespace glr {
namespace cef_client {
	
AttributeBinding::AttributeBinding(std::wstring name) : name_(name)
{
}
	
AttributeBinding::AttributeBinding(std::wstring name, std::wstring valueWstring) : name_(name), valueWstring_(valueWstring), isWstring_(true)
{
}

AttributeBinding::AttributeBinding(std::wstring name, std::string valueString) : name_(name), valueString_(valueString), isString_(true)
{
}

AttributeBinding::AttributeBinding(std::wstring name, int valueInt) : name_(name), valueInt_(valueInt), isInt_(true)
{
}

AttributeBinding::AttributeBinding(std::wstring name, uint valueUint) : name_(name), valueUint_(valueUint), isUint_(true)
{
}

AttributeBinding::AttributeBinding(std::wstring name, bool valueBool) : name_(name), valueBool_(valueBool), isBool_(true)
{
}

AttributeBinding::AttributeBinding(std::wstring name, float valueFloat) : name_(name), valueFloat_(valueFloat), isFloat_(true)
{
}

AttributeBinding::AttributeBinding(std::wstring name, double valueDouble) : name_(name), valueDouble_(valueDouble), isDouble_(true)
{
}

//AttributeBinding::AttributeBinding(std::wstring name, ObjectBinding* valueObject) : name_(name), valueObject_(valueObject), isObject_(true)
//{
//}

AttributeBinding::AttributeBinding(std::wstring name, CefRefPtr<CefV8Value> valueV8) : name_(name), valueV8_(valueV8), isV8_(true)
{
}

AttributeBinding::~AttributeBinding()
{
}

}
}
