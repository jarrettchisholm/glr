#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "ITerrain.hpp"
#include "BasicSceneNode.hpp"

namespace glr
{
namespace terrain
{

class Terrain : public virtual ITerrain, public BasicSceneNode
{
public:
	Terrain(Id id, glw::IOpenGlDevice* openGlDevice);
	Terrain(Id id, std::string name, glw::IOpenGlDevice* openGlDevice);
	Terrain(Id id, std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice);
	Terrain(Id id, const Terrain& other);
	virtual ~Terrain();

	virtual void render();
	virtual bool isDirty() const;

private:
	void initialize();
	
	bool isDirty_;
};

}
}

#endif /* TERRAIN_H_ */
