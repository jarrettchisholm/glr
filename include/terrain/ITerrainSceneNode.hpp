#ifndef ITERRAINSCENENODE_H_
#define ITERRAINSCENENODE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "ISceneNode.hpp"

namespace glr
{
namespace terrain
{

struct Point
{
	glmd::float32 density;
	glm::vec3 pos;
};

struct Block
{
	Point points[2][2][2];
	glmd::int32 index;
	glm::vec3 meshPoint;
	glm::vec3 meshPointNormal;
};

class ITerrainSceneNode : public virtual ISceneNode
{
public:
	virtual ~ITerrainSceneNode()
	{
	}
	;
	
	virtual glm::detail::int32 getGridX() const = 0;
	virtual glm::detail::int32 getGridY() const = 0;
	virtual glm::detail::int32 getGridZ() const = 0;
};

}
}

#endif /* ITERRAINSCENENODE_H_ */

