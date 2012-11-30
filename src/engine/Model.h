/*
 * Model.h
 *
 *  Created on: 2012-09-20
 *      Author: jarrett
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <string>

namespace icee {

namespace engine {

class Model {
public:
	Model();
	Model(const std::string filename);
	virtual ~Model();
	
	void render();

protected:
	#define aisgl_min(x,y) (x<y?x:y)
	#define aisgl_max(x,y) (y>x?y:x)
	
	// the global Assimp scene object
	const aiScene* scene;
	uint32 scene_list;
	aiVector3D scene_min, scene_max, scene_center;
	

	// temporary method
	void setLighting();
	
	void recursive_render (const aiScene *sc, const aiNode* nd);

};

}

}

#endif /* MODEL_H_ */
