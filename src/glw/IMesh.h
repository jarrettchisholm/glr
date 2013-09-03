#ifndef IMESH_H_
#define IMESH_H_

#include <map>
#include <iostream>

#include <glm/glm.hpp>

namespace glr {
namespace glw {

struct VertexBoneData
{
	glm::uvec4 boneIds;
	glm::vec4 weights;
	
	VertexBoneData()
	{
		boneIds = glm::uvec4();
		weights = glm::vec4();
	}
	
	void addBoneWeight( glm::detail::uint32 id, glm::detail::float32 weight )
	{
		for (glm::detail::uint32 i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				boneIds[i] = id;
				weights[i] = weight;
				
				//assert( weights[0] + weights[1] + weights[2] + weights[3] > 0.95f );
				assert( weights[0] + weights[1] + weights[2] + weights[3] < 1.05f );
				
				return;
			} 
		}
		
		// If we get here, we have too many bones per vertex
		assert(0);
	}

	/**
	 * The sum of all weights must be 1.0 - calling this method will ensure that this is so.
	 * 
	 * Note: You should only call this method once all of the weights for this bone have been added.
	 */
	void normalize()
	{
		glm::detail::float32 sum = weights[0] + weights[1] + weights[2] + weights[3];
		//std::cout << "lucky? " << sum << std::endl;
		if (sum > 0.95f && sum < 1.05f)
			return;
		
		glm::detail::int32 index = -1;
		
		// Find the last index to have a weight > 0
		for (glm::detail::int32 i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				index = i-1;
				break;
			} 
		}
		
		//std::cout << "not so lucky: " << index << " " << sum << std::endl;
		
		// if it's -1, we didn't find any empty weights OR the first weight value was 0.0
		if (index == -1)
			index = 3;
		
		glm::detail::float32 diff = 1.0f - sum;
		glm::detail::float32 extra = diff / (glm::detail::float32)(index+1);
		
		//std::cout << "not so lucky2: " << diff << " " << extra << std::endl;
		
		for (glm::detail::int32 i = 0; i < (index+1); i++)
		{
			weights[i] += extra;
		}
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
