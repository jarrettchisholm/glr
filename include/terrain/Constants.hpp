#ifndef TERRAIN_CONSTANTS_H_
#define TERRAIN_CONSTANTS_H_

namespace glr
{
namespace terrain
{
namespace constants
{

static const glm::detail::float32 RESOLUTION 			= 1.0f;
static const glm::detail::int32 CHUNK_SIZE 				= 16;
static const glm::detail::int32 SIZE 					= (glm::detail::int32)(CHUNK_SIZE / RESOLUTION);
static const glm::detail::int32 MAX_LENGTH 				= 256;
static const glm::detail::int32 MAX_WIDTH 				= 256;
static const glm::detail::int32 MAX_HEIGHT 				= 128;

static const glm::detail::float32 TOLERANCE_DENSITY 	= 1e-3f;
static const glm::detail::float32 EPSILON_DENSITY 		= 1e-4f;
static const glm::detail::float32 TOLERANCE_COORD 		= 1e-5f;
static const glm::detail::float32 EPSILON_VALUE 		= 0.001f;

}
}
}

#endif /* TERRAIN_CONSTANTS_H_ */
