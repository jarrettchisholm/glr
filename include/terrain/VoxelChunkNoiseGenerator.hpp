#ifndef NOISEMACHINE_H_
#define NOISEMACHINE_H_

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

#endif /* NOISEMACHINE_H_ */
