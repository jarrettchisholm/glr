#ifndef ISHADERPROGRAMMANAGER_H_
#define ISHADERPROGRAMMANAGER_H_

#include <memory>
#include <map>
#include <string>

#include "IShaderProgram.hpp"

#include "IShaderProgramBindListener.hpp"

namespace glr
{
namespace shaders
{
	
class IShaderProgramManager
{
public:
	virtual ~IShaderProgramManager()
	{
	}
	;

	virtual IShaderProgram* getShaderProgram(const std::string& filename) const = 0;

	virtual void loadShaderPrograms(const std::string& directory) = 0;
	virtual void reloadShaders() = 0;
	
	/**
	 * Adds the given IShaderProgramBindListener object to all currently active IShaderProgram objects.  It will then store
	 * the given IShaderProgramBindListener object, and each time a new IShaderProgram manager object is created, it will add
	 * the given IShaderProgramBindListener object to the newly created IShaderProgram.
	 * 
	 * @param bindListener The object implementing the IShaderProgramBindListener that we want to add as a bind listener to all new and existing IShaderPrograms.
	 */
	virtual void addDefaultBindListener(IShaderProgramBindListener* bindListener) = 0;
	/**
	 * Removes the given IShaderProgramBindListener object from all currently active IShaderProgram objects.  It will then remove
	 * the given IShaderProgramBindListener object from the IShaderProgramManager, so that each time a new IShaderProgram manager object is created, it will 
	 * no longer add the given IShaderProgramBindListener object to the newly created IShaderProgram.
	 * 
	 * @param bindListener The object implementing the IShaderProgramBindListener that we no longer want to add to newly created IShaderProgram objects, and that we want to 
	 * remove as a bind listener to all existing IShaderPrograms.
	 */
	virtual void removeDefaultBindListener(IShaderProgramBindListener* bindListener) = 0;
};

}
}

#endif /* ISHADERPROGRAMMANAGER_H_ */
