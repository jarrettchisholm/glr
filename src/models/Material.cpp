/*
 * Material.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <glm/gtc/type_ptr.hpp>

#include "../common/utilities/AssImpUtilities.h"

#include "Material.h"

namespace glr {
namespace models {
Material::Material(const aiMaterial* mtl, IOpenGlDevice* openGlDevice)
	: openGlDevice_(openGlDevice), bufferId_(0)
{
	BOOST_LOG_TRIVIAL(debug) << "loading material...";
	aiColor4D c;

	diffuse_[0] = 0.8f;
	diffuse_[1] = 0.8f;
	diffuse_[2] = 0.8f;
	diffuse_[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, diffuse_);

	//utilities::AssImpUtilities::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	specular_[0] = 0.0f;
	specular_[1] = 0.0f;
	specular_[2] = 0.0f;
	specular_[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, specular_);

	//utilities::AssImpUtilities::set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	ambient_[0] = 0.2f;
	ambient_[1] = 0.2f;
	ambient_[2] = 0.2f;
	ambient_[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, ambient_);

	//utilities::AssImpUtilities::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	emission_[0] = 0.0f;
	emission_[1] = 0.0f;
	emission_[2] = 0.0f;
	emission_[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, emission_);

	max_ = 1;
	ret1_ = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess_, &max_);
	max_ = 1;
	ret2_ = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength_, &max_);


	max_ = 1;
	if ( AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe_, &max_))
		fill_mode_ = wireframe_ ? GL_LINE : GL_FILL;
	else
		fill_mode_ = GL_FILL;

	max_ = 1;
	two_sided_true_ = aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided_, &max_);
	
	loadIntoVideoMemory();
	
	BOOST_LOG_TRIVIAL(debug) << "done loading material.";
}

void Material::loadIntoVideoMemory()
{
	MaterialData md = MaterialData();
	md.diffuse = diffuse_;
	md.specular = specular_;
	md.ambient = ambient_;
	md.emission = emission_;
	md.shininess = shininess_;
	md.strength = strength_;
	
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, sizeof(MaterialData), &md);
}

Material::~Material()
{
	openGlDevice_->releaseBufferObject( bufferId_ );
}

void Material::bind()
{
	bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
}

GLuint Material::getBufferId()
{
	return bufferId_;
}

GLuint Material::getBindPoint()
{
	return bindPoint_;
}
}
}

