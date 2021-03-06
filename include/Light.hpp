#ifndef LIGHT_H_
#define LIGHT_H_

#include "ILight.hpp"
#include "BasicSceneNode.hpp"

namespace glr
{

class Light : public virtual ILight, public BasicSceneNode
{
public:
	Light(Id id, glw::IOpenGlDevice* openGlDevice);
	Light(Id id, std::string name, glw::IOpenGlDevice* openGlDevice);
	virtual ~Light();

	// inherited from ILight
	virtual void render();

	virtual void attach(models::IModel* model);

	virtual void setLightData(LightData data);

	virtual const LightData& getLightData() const;

private:
	LightData lightData_;

	void initialize();
};

}

#endif /* LIGHT_H_ */
