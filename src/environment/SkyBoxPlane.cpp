#include "environment/SkyBoxPlane.hpp"

namespace glr
{
namespace env
{

SkyBoxPlane::SkyBoxPlane(Id id, std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode( id, std::move(name), openGlDevice )
{
	nodePos_ = glm::vec3();
	cameraPos_ = glm::vec3();
}

SkyBoxPlane::~SkyBoxPlane()
{
}

void SkyBoxPlane::move( const glm::vec3& movement )
{
	cameraPos_ = movement;
	cameraPos_ *= 2.0f;
	pos_ = glm::vec3( nodePos_ ) + glm::vec3( cameraPos_ );
}

void SkyBoxPlane::setPosition(glm::vec3& newPos)
{
	nodePos_ = newPos;
	pos_ = glm::vec3( nodePos_ ) + glm::vec3( cameraPos_ );
}

void SkyBoxPlane::setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	nodePos_ = glm::vec3(x, y, z);
	pos_ = glm::vec3( nodePos_ ) + glm::vec3( cameraPos_ );
}

void SkyBoxPlane::translate(const glm::vec3& trans, TransformSpace relativeTo)
{
	nodePos_ += trans;
	pos_ = glm::vec3( nodePos_ ) + glm::vec3( cameraPos_ );
}

void SkyBoxPlane::translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo)
{
	nodePos_ += glm::vec3(x, y, z);
	pos_ = glm::vec3( nodePos_ ) + glm::vec3( cameraPos_ );
}

}
}
