#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include <memory>
#include <string>
#include <map>
#include <mutex>

#include <GL/glew.h>

#include "IOpenGlDevice.hpp"

#include "IMaterialManager.hpp"

#include "serialize/SplitMember.hpp"

namespace glr
{
namespace glw
{

class Material;

class MaterialManager : public IMaterialManager
{
public:
	MaterialManager(IOpenGlDevice* openGlDevice);
	virtual ~MaterialManager();

	virtual IMaterial* getMaterial(const std::string& name) const;
	virtual IMaterial* addMaterial(const std::string& name);
	virtual IMaterial* addMaterial(
		const std::string& name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength,
		bool initialize = true
	);
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);

private:
	/**
	 * Required by serialization.
	 */
	MaterialManager();

	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Material> > materials_;
	
	mutable std::mutex accessMutex_;
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
	// Need to do these because boost serialization doesn't have a standard implementation for std::unique_ptr
	// Apparently, std::unique_ptr will have a serializable implementation in boost 1.56
	// TODO: Implement one myself?
	template<class Archive> void save(Archive & ar, const unsigned int version) const;
	template<class Archive> void load(Archive & ar, const unsigned int version);
};

}
}

#endif /* MATERIALMANAGER_H_ */
