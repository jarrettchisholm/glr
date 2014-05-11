#ifndef VOXELCHUNKNOISEGENERATOR_H_
#define VOXELCHUNKNOISEGENERATOR_H_

#include "IFieldFunction.hpp"
#include "VoxelChunk.hpp"

namespace glr
{
namespace terrain
{

/**
 * Will fill the VoxelChunk with noise generated using the field function.
 * 
 * Once the method is finished, the VoxelChunk will have the following dimensions:
 * (SIZE + POINT_FIELD_OFFSET + POINT_FIELD_OVERSET) x (SIZE + POINT_FIELD_OFFSET + POINT_FIELD_OVERSET)
 * 
 * We generate 'extra' dimensions of density data for the benefit of the smoothing functions - it allows them to use the extra data
 * for interpolation, etc.
 * 
 */
void generateNoise(VoxelChunk& chunk, glr::terrain::IFieldFunction& fieldFunction);

/**
 * 
 */
bool determineIfEmptyOrSolid(VoxelChunk& chunk);

}
}

#endif /* VOXELCHUNKNOISEGENERATOR_H_ */
