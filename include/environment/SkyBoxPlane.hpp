#ifndef SKYBOXPLANE_H_
#define SKYBOXPLANE_H_

#include <string>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "BasicSceneNode.hpp"

namespace glr
{
namespace env
{

class SkyBoxPlane : public BasicSceneNode
{
public:
	SkyBoxPlane(Id id, std::string name, glw::IOpenGlDevice* openGlDevice);
	virtual ~SkyBoxPlane();
	
	void move( const glm::vec3& movement );
	
	virtual void setPosition(glm::vec3& newPos);
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);

	virtual void translate(const glm::vec3& trans, TransformSpace relativeTo = TS_LOCAL);
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo = TS_LOCAL);

private:
	glm::vec3 nodePos_;
	glm::vec3 cameraPos_;
};

}
}

#endif /* SKYBOXPLANE_H_ */
