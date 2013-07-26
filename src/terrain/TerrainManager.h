/*
 * TerrainManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include "ITerrainManager.h"

#include "../glw/IOpenGlDevice.h"

#include "Terrain.h"

namespace glr {
	
class TerrainManager : public ITerrainManager {
public:
	TerrainManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~TerrainManager();

	ITerrain* loadTerrain(const std::string path);

	//void testLoadTexture();
	//void testDrawTest1();

private:	
	glw::IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Terrain> > models_;
};

}
#endif /* TERRAINMANAGER_H_ */
