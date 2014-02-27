#ifndef ILIGHT_H_
#define ILIGHT_H_

#include <vector>

#include "ISceneNode.h"


namespace glr
{

struct LightData {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
	glm::vec4 direction;
};

class ILight : public virtual ISceneNode {
public:
	virtual ~ILight()
	{
	}
	;

	virtual void setLightData(LightData data) = 0;
	// TODO: add 'set' methods for each piece of light data

	virtual const LightData& getLightData() = 0;
	// TODO: add 'get' methods for each piece of light data
};

}

#endif /* ILIGHT_H_ */
