#ifndef IMESH_H_
#define IMESH_H_

#include <map>
#include <iostream>
#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IGraphicsObject.hpp"

#include "common/logger/Logger.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

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
};

// Bone information
struct Bone
{
	std::string name;
	glm::mat4 boneOffset;
};

struct BoneData
{
	std::string name;
	std::map< std::string, glm::detail::uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
};

/**
 * Represents a Mesh which can be rendered in the scene.
 */
class IMesh : public virtual IGraphicsObject, public virtual serialize::ITextSerializable
{
public:
	virtual ~IMesh()
	{
	}
	;

	/**
	 * Will render this mesh in the scene.
	 */
	virtual void render() = 0;
	
	/**
	 * Returns a reference to the bone data.
	 */
	virtual BoneData& getBoneData() = 0;
	
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* MESH_H_ */
