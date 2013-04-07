/*
 * Model.h
 *
 *  Created on: 2012-09-20
 *      Author: jarrett
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <memory>
#include <map>
#include <string>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IModel.h"

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Animation.h"

namespace glr {
namespace models {
class Model : public IModel {
public:
	Model();
	Model(const aiScene* scene);
	virtual ~Model();

	virtual void render();

protected:
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																#define aisgl_min(x, y) (x < y ? x : y)
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																#define aisgl_max(x, y) (y > x ? y : x)

	// the global Assimp scene object
	//const aiScene* scene_;

	std::vector< std::unique_ptr<Mesh> > meshes_;
	std::vector<Texture*> textures_;
	std::vector< std::unique_ptr<Material> > materials_;
	std::vector<Animation*> animations_;

	std::vector<glm::detail::uint32> textureMap_;
	std::vector<glm::detail::uint32> materialMap_;

	glm::detail::uint32 scene_list;
	aiVector3D scene_min, scene_max, scene_center;

	void loadMeshes(const aiScene* scene);
	void loadTextures(const aiScene* scene);
	void loadMaterials(const aiScene* scene);
	void loadAnimations(const aiScene* scene);

	//void loadMeshesRecursive(const aiScene* scene, const aiNode* node);

	//void recursive_render(const aiScene *sc, const aiNode* nd);
};
}
}
#endif /* MODEL_H_ */
