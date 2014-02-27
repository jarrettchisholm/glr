/*
 * ITerrain.h
 *
 *  Created on: 2013-06-05
 *      Author: jarrett
 */

#ifndef ITERRAIN_H_
#define ITERRAIN_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "ISceneNode.h"


namespace glr {
struct TerrainData {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
	glm::vec4 direction;
};

class ITerrain : public virtual ISceneNode {
public:
	virtual ~ITerrain()
	{
	}
	;

	virtual void setTerrainData(TerrainData data) = 0;

	virtual const TerrainData& getTerrainData() = 0;
};
}
#endif /* ITERRAIN_H_ */
