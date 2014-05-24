#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IOpenGlDevice.hpp"
#include "shaders/IShaderProgram.hpp"

#include "IMaterial.hpp"
#include "MaterialData.hpp"

namespace glr
{
namespace glw
{
	
class IOpenGlDevice;
	
class Material : public IMaterial
{
public:
	Material(IOpenGlDevice* openGlDevice, std::string name, bool initialize = true);
	Material(
		IOpenGlDevice* openGlDevice,
		std::string name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength,
		bool initialize = true
	);
	virtual ~Material();

	virtual void bind() const;
	virtual GLuint getBufferId() const;
	GLuint getBindPoint() const;

	virtual void allocateVideoMemory();
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeVideoMemory();
	virtual bool isVideoMemoryAllocated() const;
	virtual void loadLocalData();
	virtual void freeLocalData();
	virtual bool isLocalDataLoaded() const;
	virtual bool isDirty() const;

	void setAmbient(const glm::vec4& ambient);
	void setDiffuse(const glm::vec4& diffuse);
	void setSpecular(const glm::vec4& specular);
	void setEmission(const glm::vec4& emission);
	void setShininess(glm::detail::float32 shininess);
	void setStrength(glm::detail::float32 strength);

	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);

	virtual const std::string& getName() const;
	void setName(std::string name);
private:
	/**
	 * Required by serialization.
	 */
	Material();

	IOpenGlDevice* openGlDevice_;
	std::string name_;
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	glm::vec4 emission_;
	glm::detail::float32 shininess_;
	glm::detail::float32 strength_;
	
	GLuint bufferId_;
	GLuint bindPoint_;
	
	bool isLocalDataLoaded_;
	bool isDirty_;
	
	friend class boost::serialization::access;
	
	template<class Archive> inline void serialize(Archive& ar, const unsigned int version);
};

}
}

#include "Material.inl"

#endif /* MATERIAL_H_ */
