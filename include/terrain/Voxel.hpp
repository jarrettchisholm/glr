#ifndef VOXEL_H_
#define VOXEL_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace glr
{
namespace terrain
{

struct Voxel
{
	glm::detail::float32 density;
};

}
}

#endif /* VOXEL_H_ */
