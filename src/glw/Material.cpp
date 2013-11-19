/*
 * Material.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "../common/utilities/AssImpUtilities.h"

#include "../exceptions/GlException.h"

#include "Material.h"

namespace glr {
namespace glw {

Material::Material(
		IOpenGlDevice* openGlDevice,
		const std::string name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
		)
	: openGlDevice_(openGlDevice), name_(name), ambient_(ambient), diffuse_(diffuse), specular_(specular), emission_(emission), shininess_(shininess), strength_(strength), bufferId_(0)
{
	LOG_DEBUG( "loading material..." );
	
	loadIntoVideoMemory();
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		std::stringstream ss;
		ss << "Error while loading material '" << name_ << "' in OpenGl: " << err.name;
		LOG_ERROR( ss.str() );
		throw exception::GlException( ss.str() );
	}
	else
	{
		std::stringstream ss;
		ss << "Successfully loaded material.  Buffer id: " << bufferId_;
		LOG_DEBUG( ss.str() );
	}
}

void Material::loadIntoVideoMemory()
{
	MaterialData md = MaterialData();
	md.ambient = ambient_;
	md.diffuse = diffuse_;
	md.specular = specular_;
	md.emission = emission_;
	//md.shininess = shininess_;
	//md.strength = strength_;
	
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, sizeof(MaterialData), &md);
}

Material::~Material()
{
	openGlDevice_->releaseBufferObject( bufferId_ );
}

void Material::bind()
{
	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
	//std::cout << "material: " << name_ << " | " << bufferId_ << " | " << bindPoint_ << std::endl;
	/*
	for (int i=0; i < 4; i++)
		std::cout << ambient_[i] << " ";
	std::cout << std::endl;
	for (int i=0; i < 4; i++)
		std::cout << diffuse_[i] << " ";
	std::cout << std::endl;
	for (int i=0; i < 4; i++)
		std::cout << specular_[i] << " ";
	std::cout << std::endl;
	for (int i=0; i < 4; i++)
		std::cout << emission_[i] << " ";
	
	std::cout << std::endl << std::endl;
	*/
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
