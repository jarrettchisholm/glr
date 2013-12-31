/*
 * GuiObject.h
 *
 *  Created on: 2012-12-30
 *	  Author: jarrett
 */

#ifdef USE_CEF

#ifndef GUIOBJECT_H_
#define GUIOBJECT_H_

#include <map>

#include "../IGuiObject.h"

namespace glr {
namespace gui {
namespace cef {
	
class GuiObject : public IGuiObject {
public:
	GuiObject(std::wstring name);
	virtual ~GuiObject();
	
	virtual void addFunction(std::wstring name, std::function<void()> function);
	virtual void addFunction(std::wstring name, std::function<int()> function);
	virtual void addFunction(std::wstring name, std::function<unsigned int()> function);
	virtual void addFunction(std::wstring name, std::function<float()> function);
	virtual void addFunction(std::wstring name, std::function<std::string()> function);
	virtual void addFunction(std::wstring name, std::function<std::wstring()> function);
	virtual void addFunction(std::wstring name, std::function<char()> function);
	virtual void addFunction(std::wstring name, std::function<bool()> function);


	virtual void addFunction(std::wstring name, std::function<void(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<int(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<unsigned int(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<float(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<std::string(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<std::wstring(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<char(std::vector<boost::any>)> function);
	virtual void addFunction(std::wstring name, std::function<bool(std::vector<boost::any>)> function);
	
	std::wstring getName();
	
	std::wstring getFunctionDefinitions();
	std::vector< std::wstring > getFunctionNames();

	boost::any processCallback(std::wstring name, std::vector< boost::any > params);

private:
	enum FunctionTypes {
		TYPE_VOID = 0,
		TYPE_INT,
		TYPE_UNSIGNED_INT,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_WSTRING,
		TYPE_CHAR,
		TYPE_BOOL,

		TYPE_WITH_PARAMETERS_VOID,
		TYPE_WITH_PARAMETERS_INT,
		TYPE_WITH_PARAMETERS_UNSIGNED_INT,
		TYPE_WITH_PARAMETERS_FLOAT,
		TYPE_WITH_PARAMETERS_STRING,
		TYPE_WITH_PARAMETERS_WSTRING,
		TYPE_WITH_PARAMETERS_CHAR,
		TYPE_WITH_PARAMETERS_BOOL
	};

	std::wstring name_;

	std::map< std::wstring, int > functionTypeMap_;

	std::map< std::wstring, std::function<void()> >				functionMapVoid_;
	std::map< std::wstring, std::function<int()> >				functionMapInt_;
	std::map< std::wstring, std::function<unsigned int()> >		functionMapUnsignedInt_;
	std::map< std::wstring, std::function<float()> >			functionMapFloat_;
	std::map< std::wstring, std::function<std::string()> > 		functionMapString_;
	std::map< std::wstring, std::function<std::wstring()> > 	functionMapWstring_;
	std::map< std::wstring, std::function<char()> >				functionMapChar_;
	std::map< std::wstring, std::function<bool()> >				functionMapBool_;

	std::map< std::wstring, std::function<void( std::vector<boost::any> )> >				functionMapWithParamatersVoid_;
	std::map< std::wstring, std::function<int( std::vector<boost::any> )> >				 	functionMapWithParamatersInt_;
	std::map< std::wstring, std::function<unsigned int( std::vector<boost::any> )> >		functionMapWithParamatersUnsignedInt_;
	std::map< std::wstring, std::function<float( std::vector<boost::any> )> >			   	functionMapWithParamatersFloat_;
	std::map< std::wstring, std::function<std::string(std::vector<boost::any> )> > 			functionMapWithParamatersString_;
	std::map< std::wstring, std::function<std::wstring(std::vector<boost::any> )> > 		functionMapWithParamatersWstring_;
	std::map< std::wstring, std::function<char( std::vector<boost::any> )> >				functionMapWithParamatersChar_;
	std::map< std::wstring, std::function<bool( std::vector<boost::any> )> >				functionMapWithParamatersBool_;


	void addFunction(std::wstring funcName);
};

}
}
}

#endif /* GUIOBJECT_H_ */

#endif
