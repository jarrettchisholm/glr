#ifndef TEXTUREDATA_H_
#define TEXTUREDATA_H_

#include <vector>
#include <string>

#include "glw/TextureSettings.hpp"
#include "glw/VertexBoneData.hpp"

namespace glr
{
namespace models
{

struct TextureData
{
	std::string filename;
	glw::TextureSettings settings;
};

}
}

#endif /* TEXTUREDATA_H_ */
