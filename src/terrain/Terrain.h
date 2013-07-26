/*
 * Terrain.h
 *
 *  Created on: 2013-06-05
 *      Author: jarrett
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ITerrain.h"
#include "BasicSceneNode.h"

namespace glr {
class Terrain : public virtual ITerrain, public BasicSceneNode {
public:
	Terrain(glw::IOpenGlDevice* openGlDevice);
	Terrain(const std::string name, glw::IOpenGlDevice* openGlDevice);
	virtual ~Terrain();


	// inherited from ITerrain
	virtual void render();

	virtual void attach(models::IModel* model);

	virtual void setTerrainData(TerrainData data);

	virtual const TerrainData& getTerrainData();

private:
	TerrainData lightData_;

	void initialize();
};
}
#endif /* TERRAIN_H_ */
