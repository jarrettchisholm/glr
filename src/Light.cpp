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

#include <boost/log/trivial.hpp>

#include "Light.h"

#include "common/math/Math.h"

namespace glr {
Light::Light(IOpenGlDevice* openGlDevice) : BasicSceneNode(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Light::Light(const std::string name, IOpenGlDevice* openGlDevice) : BasicSceneNode(name, openGlDevice)
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
	BOOST_LOG_TRIVIAL(debug) << "Light initialized.";
}

void Light::render()
{
	glm::quat xQuat = glm::angleAxis((rotation_.x) * math::DEGTORAD, 1.0f, 0.0f, 0.0f);
	glm::quat yQuat = glm::angleAxis((rotation_.y) * math::DEGTORAD, 0.0f, 1.0f, 0.0f);
	glm::quat zQuat = glm::angleAxis((rotation_.z) * math::DEGTORAD, 0.0f, 0.0f, 1.0f);

	glm::quat rotation = yQuat * xQuat; // Where to put zQuat??
	
	glm::quat temp = glm::conjugate(rotation);
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
