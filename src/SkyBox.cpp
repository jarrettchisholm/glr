#include "SkyBox.h"

#include "common/logger/Logger.h"

namespace glr {

SkyBox::SkyBox(glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

SkyBox::SkyBox(const std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(name, openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

SkyBox::~SkyBox()
{
}

void SkyBox::initialize()
{
	LOG_DEBUG( "SkyBox initialized." );
}

}
