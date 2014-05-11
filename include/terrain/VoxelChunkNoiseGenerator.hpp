#ifndef VOXELCHUNKNOISEGENERATOR_H_
#define VOXELCHUNKNOISEGENERATOR_H_

#include "IFieldFunction.hpp"
#include "VoxelChunk.hpp"

namespace glr
{
namespace terrain
{

/**
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
