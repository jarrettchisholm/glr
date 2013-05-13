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
#include "../glw/IOpenGlDevice.h"

#include "../glw/IMaterialManager.h"
#include "../glw/ITextureManager.h"
#include "../glw/IMeshManager.h"

#include "../glw/Mesh.h"
#include "../glw/Texture.h"
#include "../glw/Material.h"
#include "../glw/Animation.h"

namespace glr {
namespace models {
class Model : public IModel {
public:
	Model(glw::IOpenGlDevice* openGlDevice);
	Model(const aiScene* scene, glw::IOpenGlDevice* openGlDevice);
	virtual ~Model();

	virtual void render(shaders::IShaderProgram* shader);

protected:
#define aisgl_min(x, y) (x < y ? x : y)
#define aisgl_max(x, y) (y > x ? y : x)
	// the global Assimp scene object
	//const aiScene* scene_;

	std::vector< std::unique_ptr<glw::Mesh> > meshes_;
	std::vector<glw::Texture*> textures_;
	std::vector< std::unique_ptr<glw::Material> > materials_;
	std::vector<glw::Animation*> animations_;

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
	
private:
	void initialize();
	void destroy();

	glw::IOpenGlDevice* openGlDevice_;
	
	glw::IMeshManager* meshManager_;
	glw::IMaterialManager* materialManager_;
	glw::ITextureManager* textureManager_;

};
}
}
#endif /* MODEL_H_ */
