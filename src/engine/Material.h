/*
 * Material.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/gl.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace icee {

namespace engine {

class Material {
public:
	Material(const aiMaterial* mtl);
	virtual ~Material();

private:
	void bind();
};

}

}

#endif /* MATERIAL_H_ */
