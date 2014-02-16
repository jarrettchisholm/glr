/*
 * Light.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"

#include "Light.h"

#include "common/logger/Logger.h"

#include "common/math/Math.h"

namespace glr {

Light::Light(glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Light::Light(glm::detail::uint32 id, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Light::Light(glm::detail::uint32 id, const std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, name, openGlDevice)
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

const LightData& Light::getLightData()
{
	return lightData_;
}

}
