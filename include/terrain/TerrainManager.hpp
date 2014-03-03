#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include "ITerrainManager.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "Terrain.hpp"

namespace glr
{
	
class TerrainManager : public ITerrainManager
{
public:
	TerrainManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~TerrainManager();

	ITerrain* loadTerrain(const std::string path);

private:	
	glw::IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Terrain> > models_;
};

}

#endif /* TERRAINMANAGER_H_ */
