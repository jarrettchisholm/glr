#ifndef IMESH_H_
#define IMESH_H_

#include <map>

#include <glm/glm.hpp>

namespace glr {
namespace glw {

struct VertexBoneData
{
	glm::uvec4 boneIds;
	glm::vec4 weights;
	
	void addBoneWeight( glm::detail::uint32 id, glm::detail::float32 weight )
	{
		for (glm::detail::uint32 i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				boneIds[i] = id;
				weights[i] = weight;
				return;
			} 
		}
		
		// If we get here, we have too many bones per vertex
		assert(0);
	}
};

// Bone information
struct Bone {
	std::string name;
	glm::mat4 boneOffset;
	glm::mat4 finalTransformation;
};

struct BoneData {
	std::string name;
	std::map< std::string, glm::detail::uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
};

}
}

#endif /* MESH_H_ */
