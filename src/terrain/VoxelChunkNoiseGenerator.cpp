#include "terrain/VoxelChunkNoiseGenerator.hpp"
#include "terrain/Constants.hpp"

#include "terrain/IFieldFunction.hpp"

/**
 * Anonymous helper functions.
 */
namespace
{

void allocatePointsMemory(Points& points)
{
	// Make our blocks array the proper size

	points.resize(glr::terrain::constants::SIZE+3);
	for (glmd::int32 i = 0; i < glr::terrain::constants::SIZE+3; i++)
	{
		points[i].resize(glr::terrain::constants::SIZE+3);
		for (glmd::int32 j = 0; j < glr::terrain::constants::SIZE+3; j++)
			points[i][j].resize(glr::terrain::constants::SIZE+3);
	}
}

void computeDensities(glr::terrain::VoxelChunk& chunk, glmd::int32 x, glmd::int32 y, glmd::int32 z, glr::terrain::IFieldFunction& fieldFunction)
{
	glmd::float32 fx = (float)(chunk.getGridX() * glr::terrain::constants::SIZE + x) * glr::terrain::constants::RESOLUTION;
	glmd::float32 fy = (float)(chunk.getGridY() * glr::terrain::constants::SIZE + y) * glr::terrain::constants::RESOLUTION;
	glmd::float32 fz = (float)(chunk.getGridZ() * glr::terrain::constants::SIZE + z) * glr::terrain::constants::RESOLUTION;
	
	chunk.getPoints()[x][y][z] = fieldFunction.getNoise(fx, fy, fz);
	
	// Do I need something like this?  Error margin or something?  Not 100% why I would need something like this...
	chunk.getPoints()[x][y][z] += glr::terrain::constants::EPSILON_DENSITY;
}

void computePoints(glr::terrain::VoxelChunk& chunk, glr::terrain::IFieldFunction& fieldFunction)
{
	for (glmd::int32 x=0; x < glr::terrain::constants::SIZE+3; x++)
	{
		for (glmd::int32 y = 0; y < glr::terrain::constants::SIZE+3; y++)
		{
			for (glmd::int32 z=0; z < glr::terrain::constants::SIZE+3; z++)
			{
				computeDensities(chunk, x, y, z, fieldFunction);
			}
		}
	}
}

}

namespace glr
{
namespace terrain
{

void generateNoise(VoxelChunk& chunk, glr::terrain::IFieldFunction& fieldFunction)
{
	allocatePointsMemory(chunk.getPoints());
	
	computePoints(chunk, fieldFunction);
}

#define DENSITY_LOCAL(i, j, k) points[i][j][k]
bool determineIfEmptyOrSolid(VoxelChunk& chunk)
{
	auto points = chunk.getPoints();
	
	bool isEmpty = true;
	bool isSolid = true;

	for (glmd::int32 x=0; x < glr::terrain::constants::SIZE+3; x++)
	{
		for (glmd::int32 y=0; y < glr::terrain::constants::SIZE+3; y++)
		{
			for (glmd::int32 z=0; z < glr::terrain::constants::SIZE+3; z++)
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
