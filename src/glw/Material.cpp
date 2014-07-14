#include <sstream>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "common/logger/Logger.hpp"

#include "exceptions/GlException.hpp"
#include "exceptions/InvalidArgumentException.hpp"

#include "glw/Material.hpp"

namespace glr
{
namespace glw
{

Material::Material() : bufferId_(0)
{
	openGlDevice_ = nullptr;
	name_ = std::string();
	
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	this->addBindListener(openGlDevice_);
}

Material::Material(IOpenGlDevice* openGlDevice, std::string name) : openGlDevice_(openGlDevice), name_(std::move(name)), bufferId_(0)
{
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	this->addBindListener(openGlDevice_);
}

Material::Material(
		IOpenGlDevice* openGlDevice,
		std::string name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength,
		bool initialize
		)
	: openGlDevice_(openGlDevice), name_(std::move(name)), ambient_(ambient), diffuse_(diffuse), specular_(specular), emission_(emission), shininess_(shininess), strength_(strength), bufferId_(0)
{
	LOG_DEBUG( "loading material..." );
	
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	if (initialize)
	{
		loadLocalData();
		allocateVideoMemory();
		pushToVideoMemory();
		
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
			LOG_DEBUG( "Successfully loaded material.  Buffer id: " << bufferId_ );
		}
	}
	
	this->addBindListener(openGlDevice_);
}

void Material::pushToVideoMemory()
{
	LOG_DEBUG( "loading material '" + name_ + "' into video memory." );
	
	MaterialData md = MaterialData();
	md.ambient = ambient_;
	md.diffuse = diffuse_;
	md.specular = specular_;
	md.emission = emission_;
	//md.shininess = shininess_;
	//md.strength = strength_;
	
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialData), &md);
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		std::stringstream ss;
		ss << "Error while pushing data for material '" << name_ << "' to video memory: " << err.name;
		LOG_ERROR( ss.str() );
		throw exception::GlException( ss.str() );
	}
	else
	{
		LOG_DEBUG( "Successfully pushed data for material '" + name_ + "' to video memory.  Buffer id: " << bufferId_ );
	}
	
	isDirty_ = false;
}

void Material::pullFromVideoMemory()
{
	// TODO: Implement
}

void Material::loadLocalData()
{
	isLocalDataLoaded_ = true;
}

void Material::freeLocalData()
{
	isLocalDataLoaded_ = false;
}

void Material::freeVideoMemory()
{
	if (bufferId_ == 0)
	{
		LOG_WARN( "Cannot free video memory - buffer does not exist for material." );
		return;
	}
	
	openGlDevice_->releaseBufferObject( bufferId_ );
	
	bufferId_ = 0;
}

void Material::allocateVideoMemory()
{
	MaterialData md = MaterialData();
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, sizeof(MaterialData), &md);
	
	isVideoMemoryAllocated_ = true;
	
	LOG_DEBUG( "Successfully allocated material memory.  Buffer id: " << bufferId_ );
}

bool Material::isVideoMemoryAllocated() const
{
	return isVideoMemoryAllocated_;
}

bool Material::isLocalDataLoaded() const
{
	return isLocalDataLoaded_;
}

bool Material::isDirty() const
{
	return isDirty_;
}

Material::~Material()
{
	freeVideoMemory();
	
	if (openGlDevice_->getCurrentlyBoundMaterial() == this)
	{
		for ( auto bindListener : bindListeners_ )
		{
			bindListener->materialBindCallback( nullptr );
		}
	}
}

void Material::bind()
{
	assert(bufferId_ >= 0);
	
	// Don't bind if we are already bound
	if (openGlDevice_->getCurrentlyBoundMaterial() == this)
	{
		return;
	}
	
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
	
	// Notify all listeners that we have bound this material
	for ( auto bindListener : bindListeners_ )
	{
		bindListener->materialBindCallback( static_cast<IMaterial*>( this ) );
	}
}

GLuint Material::getBufferId() const
{
	return bufferId_;
}

GLuint Material::getBindPoint() const
{
	return bindPoint_;
}

void Material::setAmbient(const glm::vec4& ambient)
{
	ambient_ = ambient;
	isDirty_ = true;
}

void Material::setDiffuse(const glm::vec4& diffuse)
{
	diffuse_ = diffuse;
	isDirty_ = true;
}

void Material::setSpecular(const glm::vec4& specular)
{
	specular_ = specular;
	isDirty_ = true;
}

void Material::setEmission(const glm::vec4& emission)
{
	emission_ = emission;
	isDirty_ = true;
}

void Material::setShininess(glm::detail::float32 shininess)
{
	shininess_ = shininess;
	isDirty_ = true;
}

void Material::setStrength(glm::detail::float32 strength)
{
	strength_ = strength;
	isDirty_ = true;
}

const std::string& Material::getName() const
{
	return name_;
}

void Material::setName(std::string name)
{
	name_ = std::move(name);
}

void Material::addBindListener(IMaterialBindListener* bindListener)
{
	if (bindListener == nullptr)
	{
		std::string msg = std::string( "Bind listener must not be null." );
		LOG_ERROR( msg );
		throw exception::InvalidArgumentException(msg);
	}
	
	bindListeners_.push_back(bindListener);
}

void Material::removeBindListener(IMaterialBindListener* bindListener)
{
	if (bindListener == nullptr)
	{
		std::string msg = std::string( "Bind listener must not be null." );
		LOG_ERROR( msg );
		throw exception::InvalidArgumentException(msg);
	}
	
	auto it = std::find(bindListeners_.begin(), bindListeners_.end(), bindListener);

	if ( it != bindListeners_.end())
	{
		bindListeners_.erase(it);
	}
}

void Material::removeAllBindListeners()
{
	bindListeners_.clear();
}

void Material::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void Material::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void Material::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void Material::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
	loadLocalData();
}

}
}

BOOST_CLASS_EXPORT(glr::glw::IMaterial)
BOOST_CLASS_EXPORT_GUID(glr::glw::Material, "glr::glw::Material")
