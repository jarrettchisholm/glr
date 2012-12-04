/*
 * AssImpUtilities.h
 *
 *  Created on: 2011-05-01
 *      Author: jarrett
 */

#ifndef ASSIMPUTILITIES_H_
#define ASSIMPUTILITIES_H_

#include <boost/log/trivial.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../compatibility/Types.h"

namespace utilities {

using namespace compatibility;

struct Image {
	char* data;
	int width;
	int height;
};

class AssImpUtilities {
public:
	AssImpUtilities() {}
	virtual ~AssImpUtilities() {}
	
	static void color4_to_float4(const aiColor4D *c, float f[4]) {
		f[0] = c->r;
		f[1] = c->g;
		f[2] = c->b;
		f[3] = c->a;
	}
	
	static void set_float4(float f[4], float a, float b, float c, float d) {
		f[0] = a;
		f[1] = b;
		f[2] = c;
		f[3] = d;
	}
	
	

};

}

#endif /* ASSIMPUTILITIES_H_ */












