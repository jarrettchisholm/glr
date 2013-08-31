

#ifndef GLR_GLW_CONSTANTS_H_
#define GLR_GLW_CONSTANTS_H_

#include <string>

#include <glm/glm.hpp>

namespace glr {
namespace glw {
	
namespace glmd = glm::detail;

class Constants {
public:
	static const std::string MODEL_DIRECTORY;
	
	static const glmd::uint32 MAX_NUMBER_OF_BONES_PER_MESH;
};

}
}
#endif /* GLR_GLW_CONSTANTS_H_ */
