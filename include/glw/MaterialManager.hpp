#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include <GL/glew.h>

#include "IOpenGlDevice.hpp"

#include "IMaterialManager.hpp"

#include "Material.hpp"

namespace glr
{
namespace glw
{

class MaterialManager : public IMaterialManager
{
public:
	MaterialManager();
	MaterialManager(IOpenGlDevice* openGlDevice);
	virtual ~MaterialManager();

	virtual Material* getMaterial(const std::string& name) const;
	virtual Material* addMaterial(const std::string& name);
	virtual Material* addMaterial(
		const std::string& name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	);
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);

private:	
	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Material> > materials_;
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
};

}
}

#endif /* MATERIALMANAGER_H_ */
