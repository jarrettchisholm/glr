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

#include <GL/gl.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Animation.h"

#include "../common/compatibility/Types.h"

namespace icee {

namespace engine {

using namespace compatibility;

class Model {
public:
	Model();
	Model( std::shared_ptr<aiScene> scene );
	virtual ~Model();
	
	void render();

protected:
	#define aisgl_min(x,y) (x<y?x:y)
	#define aisgl_max(x,y) (y>x?y:x)
	
	// the global Assimp scene object
	//std::shared_ptr<aiScene> scene_;
	
	std::map<std::string, Mesh*> meshMap_;
	std::map<std::string, Texture*> textureMap_;
	std::map<std::string, Material*> materialMap_;
	std::map<std::string, Animation*> animationMap_;
	
	uint32 scene_list;
	aiVector3D scene_min, scene_max, scene_center;
	

	// temporary method
	void setLighting();
	
	void loadMeshes(std::shared_ptr<aiScene> scene);
	void loadTextures(std::shared_ptr<aiScene> scene);
	void loadMaterials(std::shared_ptr<aiScene> scene);
	void loadAnimations(std::shared_ptr<aiScene> scene);
	
	void recursive_render(const aiScene *sc, const aiNode* nd);

};

}

}

#endif /* MODEL_H_ */
