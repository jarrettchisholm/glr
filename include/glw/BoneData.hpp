#ifndef BONEDATA_H_
#define BONEDATA_H_

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"
#include "serialize/std/Map.hpp"

#include "Bone.hpp"

namespace glr
{
namespace glw
{

/**
 * 
 */
struct BoneData
{
	std::string name;
	std::map< std::string, glm::detail::uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
};

}
}

BOOST_SERIALIZATION_SPLIT_FREE(glr::glw::BoneData)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glr::glw::BoneData& bd, unsigned int version)
{
	ar & bd.name & bd.boneIndexMap & bd.boneTransform;
}

template<class Archive> void load(Archive& ar, glr::glw::BoneData& bd, unsigned int version)
{
	ar & bd.name & bd.boneIndexMap & bd.boneTransform;
}

}
}

#endif /* BONEDATA_H_ */
