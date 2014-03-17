#include <iostream>

#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Light.hpp"

#include "common/logger/Logger.hpp"

#include "common/math/Math.hpp"

namespace glr
{

Light::Light(Id id, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Light::Light(Id id, std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, name, openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Light::~Light()
{
}

void Light::initialize()
{
	LOG_DEBUG( "Light initialized." );
}

void Light::render()
{
	
	glm::quat temp = glm::conjugate(orientationQuaternion_);
	glm::mat4 rotMatrix = glm::mat4_cast(temp);
	//rotMatrix = glm::translate(rotMatrix, glm::vec3(-pos_.x, -pos_.y, -pos_.z));
	
	lightData_.direction = rotMatrix * lightData_.direction;
}

/**
 * Does nothing in the Light.
 */
void Light::attach(models::IModel* model)
{
}

void Light::setLightData(LightData data)
{
	lightData_ = data;
	
	//pos_ = lightData_.position;
}

const LightData& Light::getLightData() const
{
	return lightData_;
}

}
