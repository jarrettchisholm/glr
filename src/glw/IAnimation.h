#ifndef IANIMATION_H_
#define IANIMATION_H_

#include <string>

namespace glr {
namespace glw {

struct BoneNode {
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

}
}

#endif /* IANIMATION_H_ */
