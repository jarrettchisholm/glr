#ifndef TERRAINSETTINGS_H_
#define TERRAINSETTINGS_H_

namespace glr
{
namespace terrain
{

enum SmoothingAlgorithm
{
	ALGORITHM_UNKNOWN = -1,
	ALGORITHM_MARCHING_CUBES,
	ALGORITHM_DUAL_CONTOURING
};

/**
 * Used to pass in Terrain settings.
 */
struct TerrainSettings
{
	TerrainSettings() : smoothingAlgorithm(ALGORITHM_MARCHING_CUBES)
	{
	}
	
	SmoothingAlgorithm smoothingAlgorithm;
};

}
}

#endif /* TERRAINSETTINGS_H_ */
