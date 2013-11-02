#ifndef IMESH_H_
#define IMESH_H_

#include <map>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>

#include "../common/logging/Logger.h"

namespace glr {
namespace glw {

struct VertexBoneData
{
	glm::ivec4 boneIds;
	glm::vec4 weights;
	
	VertexBoneData()
	{
		boneIds = glm::ivec4(0);
		weights = glm::vec4(0.0f);
	}
	
	void addBoneWeight( glm::detail::uint32 id, glm::detail::float32 weight )
	{
		for (glm::detail::uint32 i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				boneIds[i] = id;
				weights[i] = weight;
				
				assert( weights[0] + weights[1] + weights[2] + weights[3] < 1.05f );
				
				return;
			} 
		}
		
		// If we get here, we have too many bones per vertex
		assert(0);
	}
	
	std::string toString()
	{
		std::stringstream ss;
		
		ss << "( ";
		ss << boneIds[0] << " " << weights[0];
		ss << " | ";
		ss << boneIds[1] << " " << weights[1];
		ss << " | ";
		ss << boneIds[2] << " " << weights[2];
		ss << " | ";
		ss << boneIds[3] << " " << weights[3];
		ss << " )";
		
		return ss.str();
	}

	/**
	 * The sum of all weights must be 1.0 - calling this method will ensure that this is so.
	 * 
	 * Note: You should only call this method once all of the weights for this bone have been added.
	 */
	void normalize()
	{
		/*
		glm::detail::float32 sum = weights[0] + weights[1] + weights[2] + weights[3];
		//if (sum < 0.5f)
		//	std::cout << "lucky? " << sum << std::endl;
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
		*/
		assert( weights[0] + weights[1] + weights[2] + weights[3] < 1.05f );
		assert( weights[0] + weights[1] + weights[2] + weights[3] > 0.95f );
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
