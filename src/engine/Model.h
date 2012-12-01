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

#include "Texture.h"

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
	std::shared_ptr<aiScene> scene_;
	
	std::map<std::string, Texture*> textureMap_;
	
	uint32 scene_list;
	aiVector3D scene_min, scene_max, scene_center;
	

	// temporary method
	void setLighting();
	
	void loadTextures(std::shared_ptr<aiScene> scene);
	
	void recursive_render(const aiScene *sc, const aiNode* nd);

};

}

}

#endif /* MODEL_H_ */
