#ifndef ITERRAINMANAGEREVENTLISTENER_H_
#define ITERRAINMANAGEREVENTLISTENER_H_

#include "ITerrain.hpp"

namespace glr
{
namespace terrain
{

class ITerrainManagerEventListener
{
public:
	virtual ~ITerrainManagerEventListener()
	{
	}
	;
	
	virtual void terrainAdded(ITerrain* terrain) = 0;
	virtual void terrainRemoved(ITerrain* terrain) = 0;
};

}
}

#endif /* ITERRAINMANAGEREVENTLISTENER_H_ */
