/*
 * ITerrainManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ITERRAINMANAGER_H_
#define ITERRAINMANAGER_H_

#include <string>

#include "ITerrain.h"

namespace glr {
	
namespace glmd = glm::detail;

class ITerrainManager {
public:
	virtual ~ITerrainManager()
	{
	}
	;

	virtual void loadTerrain(const std::string path) = 0;
	virtual ITerrain* getTerrain(glmd::int32 x, glmd::int32 y) = 0;
};

}
#endif /* ITERRAINMANAGER_H_ */
