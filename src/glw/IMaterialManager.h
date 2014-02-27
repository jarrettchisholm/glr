#ifndef IMATERIALMANAGER_H_
#define IMATERIALMANAGER_H_

#include <string>

#include "Material.h"

namespace glr
{
namespace glw
{

class Material;

class IMaterialManager
{
public:
	virtual ~IMaterialManager()
	{
	}
	;

	virtual Material* getMaterial(const std::string& name) = 0;
	virtual Material* addMaterial(const std::string& name) = 0;
	virtual Material* addMaterial(
		const std::string& name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	) = 0;
};

}
}

#endif /* IMATERIALMANAGER_H_ */
