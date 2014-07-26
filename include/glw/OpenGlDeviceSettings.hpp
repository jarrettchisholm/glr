#ifndef OPENGLDEVICESETTINGS_H_
#define OPENGLDEVICESETTINGS_H_

#include <string>

#include "glw/Constants.hpp"

namespace glr
{
namespace glw
{

/**
 * Used to pass in OpenGlDeviceSettings settings (so we don't have to have a method with a whole ton of parameters).
 */
struct OpenGlDeviceSettings
{
	OpenGlDeviceSettings() : defaultTextureDir(glr::glw::Constants::MODEL_DIRECTORY)
	{
	}
	
	std::string defaultTextureDir;
};

}
}

#endif /* OPENGLDEVICESETTINGS_H_ */
