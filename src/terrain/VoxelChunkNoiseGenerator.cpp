#include "terrain/VoxelChunkNoiseGenerator.hpp"
#include "terrain/VoxelChunk.hpp"
#include "terrain/Constants.hpp"

#include "terrain/IFieldFunction.hpp"

/**
 * Anonymous helper functions.
 */
namespace
{

void allocatePointsMemory(glr::terrain::Points& points)
{
	const glmd::int32 max = glr::terrain::constants::SIZE + glr::terrain::constants::POINT_FIELD_OFFSET + glr::terrain::constants::POINT_FIELD_OVERSET;
	
	// Make our blocks array the proper size
	points.resize(max);
	for (glmd::int32 i = 0; i < max; i++)
	{
		points[i].resize(max);
		for (glmd::int32 j = 0; j < max; j++)
			points[i][j].resize(max);
	}
}

void computeDensities(glr::terrain::VoxelChunk& chunk, glmd::int32 x, glmd::int32 y, glmd::int32 z, const glm::ivec3 dimensions, glr::terrain::IFieldFunction& fieldFunction)
{
	glmd::float32 fx = (glmd::float32)(chunk.gridX * glr::terrain::constants::SIZE + x) * glr::terrain::constants::RESOLUTION;
	glmd::float32 fy = (glmd::float32)(chunk.gridY * glr::terrain::constants::SIZE + y) * glr::terrain::constants::RESOLUTION;
	glmd::float32 fz = (glmd::float32)(chunk.gridZ * glr::terrain::constants::SIZE + z) * glr::terrain::constants::RESOLUTION;
	
	const glmd::int32 pointsPerDimension = glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION;
	
	// This is making sure that globally our chunks are centered at the origin ((0, 0, 0) in world coodinates)
	fx -= (glmd::float32)(pointsPerDimension * dimensions.x/2);
	fy -= (glmd::float32)(pointsPerDimension * dimensions.y/2);
	fz -= (glmd::float32)(pointsPerDimension * dimensions.z/2);
	
	const glmd::int32 xWithOffset = x + glr::terrain::constants::POINT_FIELD_OFFSET;
	const glmd::int32 yWithOffset = y + glr::terrain::constants::POINT_FIELD_OFFSET;
	const glmd::int32 zWithOffset = z + glr::terrain::constants::POINT_FIELD_OFFSET;
	
	// Do I need something like the EPSILON_DENSITY?  Error margin or something?  Not 100% why I would need something like this...
	chunk.points[xWithOffset][yWithOffset][zWithOffset] = fieldFunction.getNoise(fx, fy, fz) + glr::terrain::constants::EPSILON_DENSITY;
}

void computePoints(glr::terrain::VoxelChunk& chunk, const glm::ivec3& dimensions, glr::terrain::IFieldFunction& fieldFunction)
{
	const glmd::int32 min = -1 * glr::terrain::constants::POINT_FIELD_OFFSET;
	const glmd::int32 max = glr::terrain::constants::SIZE + glr::terrain::constants::POINT_FIELD_OVERSET;
	
	for (glmd::int32 x=min; x < max; x++)
	{
		for (glmd::int32 y=min; y < max; y++)
		{
			for (glmd::int32 z=min; z < max; z++)
			{
				computeDensities(chunk, x, y, z, dimensions, fieldFunction);
			}
		}
	}
}

}

namespace glr
{
namespace terrain
{

void generateNoise(VoxelChunk& chunk, glm::detail::int32 length, glm::detail::int32 width, glm::detail::int32 height, glr::terrain::IFieldFunction& fieldFunction)
{
	allocatePointsMemory(chunk.points);
	
	const glm::ivec3 dimensions = glm::ivec3(length, width, height);
	
	computePoints(chunk, dimensions, fieldFunction);
}

#define DENSITY_LOCAL(i, j, k) points[i][j][k]
bool determineIfEmptyOrSolid(VoxelChunk& chunk)
{
	const Points& points = chunk.points;
	
	bool isEmpty = true;
	bool isSolid = true;

	const glmd::int32 max = constants::SIZE + constants::POINT_FIELD_OFFSET + constants::POINT_FIELD_OVERSET;

	for (glmd::int32 x=0; x < max; x++)
	{
		for (glmd::int32 y=0; y < max; y++)
		{
			for (glmd::int32 z=0; z < max; z++)
			{
				if (isEmpty)
				{
					if (DENSITY_LOCAL(x, y, z) <= 0.0f)
						isEmpty = false;
				}

				if (isSolid)
				{
					if (DENSITY_LOCAL(x, y, z) > 0.0f)
						isSolid = false;
				}

				if (!isEmpty && !isSolid)
					return false;
			}
		}
	}

	if (isEmpty && isSolid)
		assert(0);

	return true;
}

}
}
