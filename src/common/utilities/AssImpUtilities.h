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

#include <glm/glm.hpp>

namespace utilities {
class AssImpUtilities {
public:
AssImpUtilities()
{
}
virtual ~AssImpUtilities()
{
}

static void color4_to_vec4(const aiColor4D*c, glm::vec4 v)
{
	v[0] = c->r;
	v[1] = c->g;
	v[2] = c->b;
	v[3] = c->a;
}

static void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
};
}

#endif /* ASSIMPUTILITIES_H_ */












