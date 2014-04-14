#ifndef BONENODE_H_
#define BONENODE_H_

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"
#include "serialize/glm/Mat4.hpp"

namespace glr
{
namespace glw
{

/**
 * 
 */
struct BoneNode
{
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

}
}

BOOST_SERIALIZATION_SPLIT_FREE(glr::glw::BoneNode)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glr::glw::BoneNode& bn, unsigned int version)
{
	auto size = bn.children.size();
	ar & bn.name & bn.transformation;
	ar & size;
	for (auto& child : bn.children)
	{
		ar & child;
	}
}

template<class Archive> void load(Archive& ar, glr::glw::BoneNode& bn, unsigned int version)
{
	std::vector< glr::glw::BoneNode >::size_type size = 0;
	
	ar & bn.name & bn.transformation;
	ar & size;
	for (glm::detail::uint32 i=0; i < size; i++)
	{
		glr::glw::BoneNode node = glr::glw::BoneNode();
		ar & node;
		bn.children.push_back( node );
	}
}

}
}

#endif /* BONENODE_H_ */
