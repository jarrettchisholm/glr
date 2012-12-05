/*
 * Material.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "../common/utilities/AssImpUtilities.h"

#include "Material.h"

namespace icee {

namespace engine {

Material::Material(const aiMaterial* mtl) {
	aiColor4D c;

	diffuse_[0] = 0.8f;
	diffuse_[1] = 0.8f;
	diffuse_[2] = 0.8f;
	diffuse_[3] = 1.0f;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &c))
		utilities::AssImpUtilities::color4_to_float4(&c, diffuse_);

	//utilities::AssImpUtilities::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	specular_[0] = 0.0f;
	specular_[1] = 0.0f;
	specular_[2] = 0.0f;
	specular_[3] = 1.0f;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &c))
		utilities::AssImpUtilities::color4_to_float4(&c, specular_);
	
	//utilities::AssImpUtilities::set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	ambient_[0] = 0.2f;
	ambient_[1] = 0.2f;
	ambient_[2] = 0.2f;
	ambient_[3] = 1.0f;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &c))
		utilities::AssImpUtilities::color4_to_float4(&c, ambient_);	

	//utilities::AssImpUtilities::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	emission_[0] = 0.0f;
	emission_[1] = 0.0f;
	emission_[2] = 0.0f;
	emission_[3] = 1.0f;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &c))
		utilities::AssImpUtilities::color4_to_float4(&c, emission_);

	max_ = 1;
	ret1_ = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess_, &max_);
	max_ = 1;
	ret2_ = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength_, &max_);
	

	max_ = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe_, &max_))
		fill_mode_ = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode_ = GL_FILL;

	max_ = 1;
	two_sided_true_ = aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided_, &max_);
}

Material::~Material() {
}

void Material::bind() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_);
	

	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess_ * strength_);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		glm::vec4 temp;
		temp[0] = 0.0f;
		temp[1] = 0.0f;
		temp[2] = 0.0f;
		temp[3] = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
	}

	glPolygonMode(GL_FRONT_AND_BACK, fill_mode_);

	if((AI_SUCCESS == two_sided_true_) && two_sided_)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}


}

}

