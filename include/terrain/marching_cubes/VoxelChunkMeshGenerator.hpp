#ifndef MARCHINGCUBESVOXELCHUNKMESHGENERATOR_H_
#define MARCHINGCUBESVOXELCHUNKMESHGENERATOR_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "../IVoxelChunkMeshGenerator.hpp"
#include "../VoxelChunk.hpp"

namespace glr
{
namespace terrain
{
namespace marching_cubes
{

namespace glmd = glm::detail;

class VoxelChunkMeshGenerator : public IVoxelChunkMeshGenerator
{
public:
	VoxelChunkMeshGenerator();
	virtual ~VoxelChunkMeshGenerator();

	/**
	 * Will generate a mesh of the provided VoxelChunk, and put the data in the provided vectors (vertices, normals, and textureBlendingValues).  This function
	 * will use the Marching Cubes algorithm to accomplish the smoothing of the density field data.
	 */
	virtual void generateMesh(VoxelChunk& chunk, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec4>& textureBlendingValues) const;
	
private:
	struct Point
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glmd::float32 density;
	};
	
	struct Block
	{
		Point points[2][2][2];
	};
	
	typedef std::vector< std::vector< std::vector<Block> > > Blocks;
	
	glm::vec3 vertexInterp(double isolevel, const glm::vec3& p1, const glm::vec3& p2, double valp1, double valp2) const;
	glm::vec3 calculateNormal(int x1, int y1, int z1, int x2, int y2, int z2, const Points& densityValues) const;
	glm::vec3 calculateGradientVector(int x, int y, int z, const Points& densityValues) const;
	//glm::vec3 calculateNormal(int x, int y, int z, const Points& densityValues) const;
	glm::vec3 calculateSimpleNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) const;
	/**
	 * Determines whether the points provided define a a fully solid space or a totally empty space.
	 * 
	 * @return True if space is totally empty or solid; false otherwise.
	 */
	bool isEmptyOrSolid(const Points& points) const;
	
	/**
	 * Calculate the normal for the provided point.
	 */
	glm::vec3 calculateNormal(const glm::vec3& point, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const;
	
	/**
	 * Generate the 3 points for a triangle along the y coordinate (will move along the xz plane at point y).  Will generate
	 * up to 3 points per block.
	 */
	void generateTriangles(Blocks& blocks, const Points& points, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glmd::int32 y) const;
	
	/**
	 * Set the densities and positions for the blocks, using the provided points (density values) and the grid coordinates.
	 */
	void setDensitiesAndPositions(Blocks& blocks, const Points& points, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ) const;
	
	/**
	 * Will resize the provided Blocks 3D vector to the appropriate size.
	 * 
	 * @param blocks
	 */
	void resizeBlocks(Blocks& blocks) const;
};

}
}
}

#endif /* MARCHINGCUBESVOXELCHUNKMESHGENERATOR_H_ */
