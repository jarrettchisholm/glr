#ifndef TEXTURE2DARRAY_H_
#define TEXTURE2DARRAY_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/IShaderProgram.h"
#include "IOpenGlDevice.h"
#include "ITexture.h"

#include "../common/utilities/ImageLoader.h"
#include "../common/utilities/Macros.h"

namespace glr {
namespace glw {

namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture2DArray : public ITexture {
public:
	Texture2DArray(IOpenGlDevice* openGlDevice, const std::string name, const TextureSettings settings = TextureSettings());
	Texture2DArray(std::vector<utilities::Image*> images, IOpenGlDevice* openGlDevice, const std::string name, const TextureSettings settings = TextureSettings());
	virtual ~Texture2DArray();

	virtual void bind(GLuint texturePosition = 0);
	
	GLuint getBufferId();
	GLuint getBindPoint();
	
	void setData(std::vector<utilities::Image*> images);
	std::vector<utilities::Image>& getData();
	void pushToVideoMemory();
	void pullFromVideoMemory();
	void freeLocalData();
	void freeVideoMemory();
	void allocateVideoMemory();

	GETSET(std::string, name_, Name)

private:
	std::string name_;
	IOpenGlDevice* openGlDevice_;
	GLuint bufferId_;
	GLuint bindPoint_;
	
	TextureSettings settings_;
	
	std::vector<utilities::Image> images_;
	
	//GLuint textureId_;

	void loadIntoVideoMemory();
	
	/**
	 * Helper method - returns true if all of the images in the vector are the same internal format, and false otherwise.
	 */
	bool areImagesSameFormat();
};

}
}
#endif /* TEXTURE2DARRAY_H_ */
