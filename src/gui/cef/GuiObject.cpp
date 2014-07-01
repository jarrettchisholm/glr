#ifdef USE_CEF

#include <sstream>
#include <utility>

#include "gui/cef/GuiObject.hpp"

#include "exceptions/Exception.hpp"

#include "common/logger/Logger.hpp"
#include "common/utilities/StringUtilities.hpp"

namespace glr
{
namespace gui
{
namespace cef
{

GuiObject::GuiObject(std::wstring name) : name_(std::move(name))
{
}

GuiObject::~GuiObject()
{
}

namespace
{

inline void checkFunctionWithNameDoesntExist(const std::map< std::wstring, int >& functionTypeMap, const std::wstring& name)
{
	auto it = functionTypeMap.find(name);
	if (it != functionTypeMap.end())
	{
		std::string msg = std::string("Function with name '") + utilities::toString(name) + std::string("' already exists.");
		LOG_ERROR(msg);
		throw exception::Exception(msg);
	}
}

}

void GuiObject::addFunction(const std::wstring& name, std::function<void()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_VOID;
	functionMapVoid_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<int()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_INT;
	functionMapInt_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<unsigned int()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_UNSIGNED_INT;
	functionMapUnsignedInt_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<float()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_FLOAT;
	functionMapFloat_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<std::wstring()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WSTRING;
	functionMapWstring_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<std::string()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_STRING;
	functionMapString_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<char()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_CHAR;
	functionMapChar_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<bool()> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_BOOL;
	functionMapBool_[name] = std::move(function);
}


void GuiObject::addFunction(const std::wstring& name, std::function<void(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_VOID;
	functionMapWithParamatersVoid_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<int(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_INT;
	functionMapWithParamatersInt_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<unsigned int(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_UNSIGNED_INT;
	functionMapWithParamatersUnsignedInt_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<float(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_FLOAT;
	functionMapWithParamatersFloat_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<std::string(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_WSTRING;
	functionMapWithParamatersString_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<std::wstring(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_STRING;
	functionMapWithParamatersWstring_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<char(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_CHAR;
	functionMapWithParamatersChar_[name] = std::move(function);
}

void GuiObject::addFunction(const std::wstring& name, std::function<bool(std::vector<boost::any>)> function)
{
	checkFunctionWithNameDoesntExist(functionTypeMap_, name);
	
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_BOOL;
	functionMapWithParamatersBool_[name] = std::move(function);
}

std::vector< std::wstring > GuiObject::getFunctionNames() const
{
	auto names = std::vector< std::wstring >();
	
	for ( auto& it : functionTypeMap_ )
	{
		names.push_back( it.first );
	}
	
	return names;
}

std::wstring GuiObject::getFunctionDefinitions() const
{
	std::wstringstream definitions;
	
	// Number of functions
	definitions << functionTypeMap_.size();
	
	for ( auto& it : functionTypeMap_ )
	{
		const std::wstring name = it.first;
		const int type = it.second;
		
		definitions << ",";
		
		if (type == FunctionTypes::TYPE_VOID || type == FunctionTypes::TYPE_WITH_PARAMETERS_VOID)
		{
			definitions << " " << FunctionTypes::TYPE_VOID << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_INT || type == FunctionTypes::TYPE_WITH_PARAMETERS_INT)
		{
			definitions << " " << FunctionTypes::TYPE_INT << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_UNSIGNED_INT || type == FunctionTypes::TYPE_WITH_PARAMETERS_UNSIGNED_INT)
		{
			definitions << " " << FunctionTypes::TYPE_UNSIGNED_INT << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_FLOAT || type == FunctionTypes::TYPE_WITH_PARAMETERS_FLOAT)
		{
			definitions << " " << FunctionTypes::TYPE_FLOAT << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_STRING || type == FunctionTypes::TYPE_WITH_PARAMETERS_STRING)
		{
			definitions << " " << FunctionTypes::TYPE_STRING << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_WSTRING || type == FunctionTypes::TYPE_WITH_PARAMETERS_WSTRING)
		{
			definitions << " " << FunctionTypes::TYPE_WSTRING << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_CHAR || type == FunctionTypes::TYPE_WITH_PARAMETERS_CHAR)
		{
			definitions << " " << FunctionTypes::TYPE_CHAR << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_BOOL || type == FunctionTypes::TYPE_WITH_PARAMETERS_BOOL)
		{
			definitions << " " << FunctionTypes::TYPE_BOOL << " " << name << " 0";
		}
	}
	
	return definitions.str();
}

const std::wstring& GuiObject::getName() const
{
	return name_;
}

boost::any GuiObject::processCallback(const std::wstring& name, const std::vector< boost::any >& params)
{	
	auto it = functionTypeMap_.find( name );
	if (it == functionTypeMap_.end())
	{
		std::string msg = std::string("Function with name '") + utilities::toString(name) + std::string("' doesn't exist.");
		LOG_ERROR(msg);
		throw exception::Exception(msg);
	}
	
	const int type = it->second;

	boost::any variant = boost::any();

	switch ( type )
	{
		case FunctionTypes::TYPE_VOID:
		{
			functionMapVoid_[name]();
		}
		break;
	
		case FunctionTypes::TYPE_INT:
		{
			int r = functionMapInt_[name]();
			variant = boost::any(r);
		}
		break;
		
		case FunctionTypes::TYPE_UNSIGNED_INT:
		{
			int r = functionMapUnsignedInt_[name]();
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_FLOAT:
		{
			float r = functionMapFloat_[name]();
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_STRING:
		{
			std::string r = functionMapString_[name]();
			variant = boost::any(r);
		}
		break;
		
		case FunctionTypes::TYPE_WSTRING:
		{
			std::wstring r = functionMapWstring_[name]();
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_CHAR:
		{
			char r = functionMapChar_[name]();
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_BOOL:
		{
			bool r = functionMapBool_[name]();
			variant = boost::any(r);
		}
		break;
	
	
		// Lambda functions with parameters
		case FunctionTypes::TYPE_WITH_PARAMETERS_VOID:
		{
			functionMapWithParamatersVoid_[name](params);
		}
		break;
	
		case FunctionTypes::TYPE_WITH_PARAMETERS_INT:
		{
			int r = functionMapWithParamatersInt_[name](params);
			variant = boost::any(r);
		}
		break;
		
		case FunctionTypes::TYPE_WITH_PARAMETERS_UNSIGNED_INT:
		{
			int r = functionMapWithParamatersUnsignedInt_[name](params);
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_WITH_PARAMETERS_FLOAT:
		{
			float r = functionMapWithParamatersFloat_[name](params);
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_WITH_PARAMETERS_STRING:
		{
			std::string r = functionMapWithParamatersString_[name](params);
			variant = boost::any(r);
		}
		break;
		
		case FunctionTypes::TYPE_WITH_PARAMETERS_WSTRING:
		{
			std::wstring r = functionMapWithParamatersWstring_[name](params);
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_WITH_PARAMETERS_CHAR:
		{
			char r = functionMapWithParamatersChar_[name](params);
			variant = boost::any(r);
		}
		break;
	
		case FunctionTypes::TYPE_WITH_PARAMETERS_BOOL:
		{
			bool r = functionMapWithParamatersBool_[name](params);
			variant = boost::any(r);
		}
		break;
	}

	return variant;
}

}
}
}

#endif /* USE_CEF */
