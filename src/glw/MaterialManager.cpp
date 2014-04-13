#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "glw/MaterialManager.hpp"


namespace glr
{
namespace glw
{

MaterialManager::MaterialManager()
{
	openGlDevice_ = nullptr;
}

MaterialManager::MaterialManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

MaterialManager::~MaterialManager()
{
}

Material* MaterialManager::getMaterial(const std::string& name) const
{
	auto it = materials_.find(name);
	if ( it != materials_.end() )
	{
		LOG_DEBUG( "Material '" + name + "' found." );
		return it->second.get();
	}

	LOG_DEBUG( "Material '" + name + "' not found." );
	
	return nullptr;
}

Material* MaterialManager::addMaterial(const std::string& name)
{
	LOG_DEBUG( "Loading material '" + name + "'." );

	auto it = materials_.find(name);
	if ( it != materials_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Material '" + name + "' already exists." );
		return it->second.get();
	}

	materials_[name] = std::unique_ptr<Material>(new Material(openGlDevice_, name));

	return materials_[name].get();
}

Material* MaterialManager::addMaterial(
		const std::string& name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	)
{
	LOG_DEBUG( "Loading material '" + name + "'." );

	auto it = materials_.find(name);
	if ( it != materials_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Material '" + name + "' already exists." );
		return it->second.get();
	}

	materials_[name] = std::unique_ptr<Material>(new Material(openGlDevice_, name, ambient, diffuse, specular, emission, shininess, strength));

	return materials_[name].get();
}

void MaterialManager::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void MaterialManager::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void MaterialManager::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void MaterialManager::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
}

template<class Archive> void MaterialManager::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<MaterialManager, IMaterialManager>(
		static_cast<MaterialManager*>(nullptr),
		static_cast<IMaterialManager*>(nullptr)
	);
	/*
	ar & name_;
	ar & vertices_;
	ar & normals_;
	ar & textureCoordinates_;
	ar & colors_;
	ar & vertexBoneData_;
	ar & boneData_;
	*/
}

}
}

BOOST_CLASS_EXPORT(glr::glw::IMaterialManager)
BOOST_CLASS_EXPORT_GUID(glr::glw::MaterialManager, "glr::glw::MaterialManager")
