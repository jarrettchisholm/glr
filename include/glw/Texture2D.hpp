#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "shaders/IShaderProgram.hpp"
#include "IOpenGlDevice.hpp"
#include "ITexture.hpp"

#include "common/utilities/Macros.hpp"

namespace glr
{
namespace glw
{

namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture2D : public ITexture
{
public:
	Texture2D(IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings = TextureSettings());
	Texture2D(utilities::Image* image, IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings = TextureSettings());
	virtual ~Texture2D();

	virtual void bind(GLuint texturePosition = 0) const;
	
	GLuint getBufferId() const;
	GLuint getBindPoint() const;
	
	/**
	 * If the image is of the same format, width, and height of the previously uploaded image, you
	 * call 'pushToVideoMemory()' after calling this method.
	 * 
	 * If the image is NOT of the same format, width, or height of the previously uploaded image, you
	 * need to call 'freeVideoMemory()', 'allocateVideoMemory()', and then 'pushToVideoMemory()' in order to
	 * get the image data uploaded to the video card properly.
	 * 
	 * @param image
	 */
	void setData(utilities::Image* image);
	
	utilities::Image* getData();
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeLocalData();
	virtual void freeVideoMemory();
	virtual void allocateVideoMemory();

	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);

	GETSET(std::string, name_, Name)

private:
	/**
	 * Required by serialization.
	 */
	Texture2D();

	IOpenGlDevice* openGlDevice_;
	std::string name_;
	TextureSettings settings_;
	
	utilities::Image image_;
	GLint internalFormat_;
	
	GLuint bufferId_;
	GLuint bindPoint_;
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
};

}
}

#endif /* TEXTURE2D_H_ */
