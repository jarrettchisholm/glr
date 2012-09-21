/*
 * AssetManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../common/compatibility/Types.h"

namespace icee {

namespace engine {
	
using namespace compatibility;

class AssetManager {
public:
	AssetManager();
	virtual ~AssetManager();

	int loadAsset(const char* path);
	void recursive_render (const aiScene *sc, const aiNode* nd);
	
	void drawAll();
	
private:
	// the global Assimp scene object
	const aiScene* scene = 0;
	uint32 scene_list = 0;
	aiVector3D scene_min, scene_max, scene_center;
	
	aiLogStream stream;
	
	// current rotation angle
	static float angle;
	
	#define aisgl_min(x,y) (x<y?x:y)
	#define aisgl_max(x,y) (y>x?y:x)
	
	// temporary method
	void setLighting();
	
};

}

}

#endif /* ASSETMANAGER_H_ */
