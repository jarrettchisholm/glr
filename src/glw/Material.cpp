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
namespace glw {
Material::Material(
		IOpenGlDevice* openGlDevice,
		const std::string path,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
		)
	: openGlDevice_(openGlDevice), ambient_(ambient), diffuse_(diffuse), specular_(specular), emission_(emission), shininess_(shininess), strength_(strength), bufferId_(0)
{
	BOOST_LOG_TRIVIAL(debug) << "loading material...";
	
	loadIntoVideoMemory();
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// TODO: throw error
		BOOST_LOG_TRIVIAL(error) << "Error loading material in opengl";
		BOOST_LOG_TRIVIAL(error) << "OpenGL error: " << err.name;
	}
	else
	{
		BOOST_LOG_TRIVIAL(debug) << "Successfully loaded material.  Buffer id: " << bufferId_;
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
	bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
	
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

