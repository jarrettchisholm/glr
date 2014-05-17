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

enum LevelOfDetail
{
	LOD_UNKNOWN = -1,
	LOD_LOWEST,
	LOD_LOW,
	LOD_MEDIUM,
	LOD_HIGH,
	LOD_HIGHEST
};

/**
 * Used to pass in Terrain settings.
 */
struct TerrainSettings
{
	TerrainSettings() 
		: smoothingAlgorithm(ALGORITHM_MARCHING_CUBES), length(4), width(4), height(2),
		maxViewDistance(256), maxLevelOfDetail(LOD_HIGHEST), minLevelOfDetail(LOD_LOWEST)
	{
	}
	
	SmoothingAlgorithm smoothingAlgorithm;
	glm::detail::int32 length;
	glm::detail::int32 width;
	glm::detail::int32 height;
	glm::detail::int32 maxViewDistance;
	LevelOfDetail maxLevelOfDetail;
	LevelOfDetail minLevelOfDetail;
};

}
}

#endif /* TERRAINSETTINGS_H_ */
