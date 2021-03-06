#ifndef IGUICOMPONENT_H_
#define IGUICOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IGuiObject.hpp"

namespace glr
{
namespace gui
{

/**
 * 
 */
class IGuiComponent
{
public:
	virtual ~IGuiComponent()
	{
	}
	;

	/**
	 * Load the resources necessary for this IGuiComponent object, and also binds
	 * GUIObject objects and methods.
	 * 
	 * Note that any GUIObjects created AFTER load() has been called will not have their
	 * objects / methods bound.
	 */
	virtual void load() = 0;
	
	/**
	 * Frees the resources used by this IGuiComponent object.  Any bindings are also removed.
	 * 
	 * Note that any GUIObjects still existing are not destroyed - you may call load() again after
	 * unload(), and your GUIObjects will be rebound.
	 */
	virtual void unload() = 0;

	/**
	 * Execute the code given by script on the current IGuiComponent object.
	 * 
	 * @param script The code to execute.
	 */
	virtual void executeScript(const std::wstring& script) = 0;

	virtual bool isVisible() const = 0;
	virtual void setVisible(bool isVisible) = 0;

	/**
	 * Create a GUIObject that your gui can use to talk to your application.
	 * 
	 * This is essentially how you 'bind' objects and methods from your gui to your application.
	 *
	 * @param name The name associated with this GUIObject
	 * @return A pointer to an IGuiObject, or nullptr if the GUIObject could not be created successfully.
	 */
	virtual IGuiObject* createGuiObject(const std::wstring& name) = 0;

	virtual IGuiObject* getGuiObject(const std::wstring& name) const = 0;
};

}
}

#endif /* IGUICOMPONENT_H_ */
