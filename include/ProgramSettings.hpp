#ifndef PROGRAMSETTINGS_H_
#define PROGRAMSETTINGS_H_

#include <string>

namespace glr
{

/**
 * Used to pass in GlrProgram settings (so we don't have to have a method with a whole ton of parameters).
 */
struct ProgramSettings
{
	ProgramSettings() : defaultTextureDir("")
	{
	}
	
	std::string defaultTextureDir;
};

}

#endif /* PROGRAMSETTINGS_H_ */
