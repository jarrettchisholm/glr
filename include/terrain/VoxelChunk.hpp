#ifndef VOXELCHUNK_H_
#define VOXELCHUNK_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "common/utilities/Macros.hpp"

namespace glmd = glm::detail;

typedef std::vector< std::vector< std::vector<glmd::float32> > > Points;

namespace glr
{
namespace terrain
{

class VoxelChunk
{
public:
	VoxelChunk() {};
	VoxelChunk(glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ) : gridX_(gridX), gridY_(gridY), gridZ_(gridZ)
	{};
	virtual ~VoxelChunk() {};
	
	//GET_REF(Points, points_, Points)
	SET(Points, points_, Points)

	Points& getPoints()
	{
		return points_;
	}
	
	GETSET(glmd::int32, gridX_, GridX)
	GETSET(glmd::int32, gridY_, GridY)
	GETSET(glmd::int32, gridZ_, GridZ)
	
protected:
	glmd::int32 gridX_, gridY_, gridZ_;
	Points points_;
};

}
}

#endif /* VOXELCHUNK_H_ */
