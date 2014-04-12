#ifndef VERTEXBONEDATA_H_
#define VERTEXBONEDATA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"
#include "serialize/glm/Vec4.hpp"
#include "serialize/glm/IVec4.hpp"

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

}
}

BOOST_SERIALIZATION_SPLIT_FREE(glr::glw::VertexBoneData)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glr::glw::VertexBoneData& vbw, unsigned int version)
{
	ar & vbw.boneIds & vbw.weights;
}

template<class Archive> void load(Archive& ar, glr::glw::VertexBoneData& vbw, unsigned int version)
{
	ar & vbw.boneIds & vbw.weights;
}

}
}

#endif /* VERTEXBONEDATA_H_ */
