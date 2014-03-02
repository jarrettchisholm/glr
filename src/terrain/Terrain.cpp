#include <iostream>

#include "../Configure.h"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"

#include "Terrain.h"

#include "../common/logger/Logger.hpp"

#include "../common/math/Math.hpp"

namespace glr
{

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
	LOG_DEBUG( "Terrain initialized." );
}

void Terrain::render()
{
	glm::quat xQuat = glm::angleAxis(glm::radians(rotation_.x), 1.0f, 0.0f, 0.0f);
	glm::quat yQuat = glm::angleAxis(glm::radians(rotation_.y), 0.0f, 1.0f, 0.0f);
	glm::quat zQuat = glm::angleAxis(glm::radians(rotation_.z), 0.0f, 0.0f, 1.0f);

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
