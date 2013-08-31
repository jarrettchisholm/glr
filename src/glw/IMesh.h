#ifndef IMESH_H_
#define IMESH_H_

#include <map>

namespace glr {
namespace glw {

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
