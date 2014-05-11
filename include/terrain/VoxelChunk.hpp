#ifndef VOXELCHUNK_H_
#define VOXELCHUNK_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "common/utilities/Macros.hpp"

namespace glmd = glm::detail;

namespace glr
{
namespace terrain
{

typedef std::vector< std::vector< std::vector<glmd::float32> > > Points;

struct VoxelChunk
{
	glmd::int32 gridX;
	glmd::int32 gridY;
	glmd::int32 gridZ;
	Points points;
	
	VoxelChunk() : gridX(0), gridY(0), gridZ(0)
	{};
	VoxelChunk(glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ) : gridX(gridX), gridY(gridY), gridZ(gridZ)
	{};
};

}
}

#endif /* VOXELCHUNK_H_ */
