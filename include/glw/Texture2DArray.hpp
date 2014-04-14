#ifndef TEXTURE2DARRAY_H_
#define TEXTURE2DARRAY_H_

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "shaders/IShaderProgram.hpp"
#include "IOpenGlDevice.hpp"
#include "ITexture.hpp"

#include "common/utilities/ImageLoader.hpp"
#include "common/utilities/Macros.hpp"

namespace glr
{
namespace glw
{

namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture2DArray : public ITexture
{
public:
	Texture2DArray(IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings = TextureSettings());
	Texture2DArray(const std::vector<utilities::Image*>& images, IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings = TextureSettings());
	virtual ~Texture2DArray();

	virtual void bind(GLuint texturePosition = 0) const;
	
	GLuint getBufferId() const;
	GLuint getBindPoint() const;
	
	void setData(const std::vector<utilities::Image*>& images);
	std::vector<utilities::Image>& getData();
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeLocalData();
	virtual void freeVideoMemory();
	virtual void allocateVideoMemory();

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
	Texture2DArray();
	
	IOpenGlDevice* openGlDevice_;
	std::string name_;
	TextureSettings settings_;
	
	GLuint bufferId_;
	GLuint bindPoint_;
	
	std::vector<utilities::Image> images_;
	
	/**
	 * Helper method - returns true if all of the images in the vector are the same internal format, and false otherwise.
	 */
	bool areImagesSameFormat();
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
};

}
}

#endif /* TEXTURE2DARRAY_H_ */
