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
	
	/**
	 * Is called when a new terrain is added to the Terrain Manager.
	 * 
	 * @param terrain The new terrain that was added.
	 */
	virtual void terrainAdded(ITerrain* terrain) = 0;
	
	/**
	 * Is called when a terrain is removed from the Terrain Manager.
	 * 
	 * @param terrain The terrain that was removed.
	 */
	virtual void terrainRemoved(ITerrain* terrain) = 0;
};

}
}

#endif /* ITERRAINMANAGEREVENTLISTENER_H_ */
