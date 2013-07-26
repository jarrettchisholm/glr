/*
 * Terrain.cpp
 *
 *  Created on: 2013-06-05
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

#include "Terrain.h"

#include "common/math/Math.h"

namespace glr {
Terrain::Terrain(glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Terrain::Terrain(const std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(name, openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Terrain::~Terrain()
{
}

void Terrain::initialize()
{
	BOOST_LOG_TRIVIAL(debug) << "Terrain initialized.";
}

void Terrain::render()
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
 * Does nothing in the Terrain.
 */
void Terrain::attach(models::IModel* model)
{
}

void Terrain::setTerrainData(TerrainData data)
{
	lightData_ = data;
	
	//pos_ = lightData_.position;
}

const TerrainData& Terrain::getTerrainData()
{
	return lightData_;
}
}
