#ifndef LIGHT_H_
#define LIGHT_H_

#include "ILight.h"
#include "BasicSceneNode.h"

namespace glr
{

class Light : public virtual ILight, public BasicSceneNode
{
public:
	Light(glw::IOpenGlDevice* openGlDevice);
	Light(glm::detail::uint32 id, glw::IOpenGlDevice* openGlDevice);
	Light(glm::detail::uint32 id, const std::string name, glw::IOpenGlDevice* openGlDevice);
	virtual ~Light();

	// inherited from ILight
	virtual void render();

	virtual void attach(models::IModel* model);

	virtual void setLightData(LightData data);

	virtual const LightData& getLightData();

private:
	LightData lightData_;

	void initialize();
};

}

#endif /* LIGHT_H_ */
