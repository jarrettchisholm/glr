#ifndef IVOXELCHUNKMESHGENERATOR_H_
#define IVOXELCHUNKMESHGENERATOR_H_

#include "IFieldFunction.hpp"
#include "VoxelChunk.hpp"

namespace glr
{
namespace terrain
{

class IVoxelChunkMeshGenerator
{
public:

	/**
	 * Will generate a mesh of the provided VoxelChunk, and put the data in the provided vectors (vertices, normals, and textureBlendingValues).
	 */
	virtual void generateMesh(VoxelChunk& chunk, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec4>& textureBlendingValues) const = 0;
};

}
}

#endif /* IVOXELCHUNKMESHGENERATOR_H_ */
