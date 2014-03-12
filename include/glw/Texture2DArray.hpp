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

class Texture2DArray : public ITexture {
public:
	Texture2DArray(IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings = TextureSettings());
	Texture2DArray(const std::vector<utilities::Image*>& images, IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings = TextureSettings());
	virtual ~Texture2DArray();

	virtual void bind(GLuint texturePosition = 0);
	
	GLuint getBufferId();
	GLuint getBindPoint();
	
	void setData(const std::vector<utilities::Image*>& images);
	std::vector<utilities::Image>& getData();
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeLocalData();
	virtual void freeVideoMemory();
	virtual void allocateVideoMemory();

	GETSET(std::string, name_, Name)

private:
	std::string name_;
	IOpenGlDevice* openGlDevice_;
	GLuint bufferId_;
	GLuint bindPoint_;
	
	TextureSettings settings_;
	
	std::vector<utilities::Image> images_;
	
	/**
	 * Helper method - returns true if all of the images in the vector are the same internal format, and false otherwise.
	 */
	bool areImagesSameFormat();
};

}
}

#endif /* TEXTURE2DARRAY_H_ */
