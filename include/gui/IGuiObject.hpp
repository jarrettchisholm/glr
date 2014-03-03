#ifndef IGUIOBJECT_H_
#define IGUIOBJECT_H_

#include <functional>
#include <vector>

#include <boost/any.hpp>

namespace glr
{
namespace gui
{

class IGuiObject
{
public:
	virtual ~IGuiObject()
	{
	}
	;

	/**
	 * Adds a lambda callback function to this IGuiObject.  The function has the specified name, and it's signature
	 * is specified by the passed in std::function object.
	 * 
	 * @param function The actual function to be called.
	 */
	virtual void addFunction(std::wstring name, std::function<void()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<int()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<unsigned int()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<float()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<std::wstring()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<char()> function) = 0;
	virtual void addFunction(std::wstring name, std::function<bool()> function) = 0;

	/**
	 * Adds a lambda callback function to this IGuiObject.  The function has the specified name, and it's signature
	 * is specified by the passed in std::function object.  The signature can also have an unspecified number of
	 * parameters, implemented as boost::any objects.
	 * 
	 * @param function The actual function to be called.
	 */
	virtual void addFunction(std::wstring name, std::function<void(std::vector<boost::any>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<int(std::vector<boost::any>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<unsigned int(std::vector<boost::any>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<float(std::vector<boost::any>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<std::wstring(std::vector<boost::any>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<char(std::vector<boost::any>)> function) = 0;
	virtual void addFunction(std::wstring name, std::function<bool(std::vector<boost::any>)> function) = 0;
};

}
}

#endif /* IGUIOBJECT_H_ */
