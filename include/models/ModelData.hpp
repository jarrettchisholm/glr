#ifndef MODELDATA_H_
#define MODELDATA_H_

#include "models/MeshData.hpp"
#include "models/MaterialData.hpp"
#include "models/TextureData.hpp"
#include "glw/BoneData.hpp"

namespace glr
{
namespace models
{

struct ModelData
{
	MeshData meshData;
	MaterialData materialData;
	TextureData textureData;
	glw::BoneData boneData;
};

}
}

#endif /* MODELDATA_H_ */
