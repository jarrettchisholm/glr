#ifndef TEXTURE2DARRAY_H_
#define TEXTURE2DARRAY_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/IShaderProgram.h"
#include "IOpenGlDevice.h"

#include "../common/utilities/ImageLoader.h"
#include "../common/utilities/Macros.h"

namespace glr {
namespace glw {

namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture2DArray {
public:
	Texture2DArray(IOpenGlDevice* openGlDevice, const std::string name);
	Texture2DArray(std::vector<utilities::Image*> images, IOpenGlDevice* openGlDevice, const std::string name);
	virtual ~Texture2DArray();

	void bind(GLuint texturePosition = 0);
	
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
	
	std::vector<utilities::Image> images_;
	
	//GLuint textureId_;

	void loadIntoVideoMemory();
};

}
}
#endif /* TEXTURE2DARRAY_H_ */
