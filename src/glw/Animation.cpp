/*
 * Animation.cpp
 *
 *  Created on: 2012-05-08
 *      Author: jarrett
 */

#include "Animation.h"

namespace glr {
namespace glw {
	
Animation::Animation(
		IOpenGlDevice* openGlDevice,
		const std::string name, 
		glm::detail::float64 duration, 
		glm::detail::float64 ticksPerSecond, 
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes
	) : openGlDevice_(openGlDevice), name_(name), duration_(duration), ticksPerSecond_(ticksPerSecond), animatedBoneNodes_(animatedBoneNodes)
{
	BOOST_LOG_TRIVIAL(debug) << "loading animation...";
	
	loadIntoVideoMemory();
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// TODO: throw error
		BOOST_LOG_TRIVIAL(error) << "Error loading animation in opengl";
		BOOST_LOG_TRIVIAL(error) << "OpenGL error: " << err.name;
	}
	else
	{
		BOOST_LOG_TRIVIAL(debug) << "Successfully loaded animation.  Buffer id: " << bufferId_;
	}
}

Animation::~Animation()
{
	openGlDevice_->releaseBufferObject( bufferId_ );
}

void Animation::loadIntoVideoMemory()
{
	// TODO: implement
	
	/*
	MaterialData md = MaterialData();
	md.ambient = ambient_;
	md.diffuse = diffuse_;
	md.specular = specular_;
	md.emission = emission_;
	//md.shininess = shininess_;
	//md.strength = strength_;
	
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, sizeof(MaterialData), &md);
	*/
}

void Animation::bind()
{
	// TODO: implement
	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
}

GLuint Animation::getBufferId()
{
	return bufferId_;
}

GLuint Animation::getBindPoint()
{
	return bindPoint_;
}

}
}

