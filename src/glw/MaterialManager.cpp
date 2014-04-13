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
	boost::serialization::split_member(ar, *this, version);
}

template<class Archive> void MaterialManager::save(Archive& ar, const unsigned int version) const
{
    boost::serialization::void_cast_register<MaterialManager, IMaterialManager>(
		static_cast<MaterialManager*>(nullptr),
		static_cast<IMaterialManager*>(nullptr)
	);
	
	auto size = materials_.size();
	ar & size;
	for (auto& it : materials_)
	{
		ar & it.first & *(it.second.get());
	}
}

template<class Archive> void MaterialManager::load(Archive& ar, const unsigned int version)
{
    boost::serialization::void_cast_register<MaterialManager, IMaterialManager>(
		static_cast<MaterialManager*>(nullptr),
		static_cast<IMaterialManager*>(nullptr)
	);

    std::map< std::string, std::unique_ptr<Material> >::size_type numMaterials = 0;
    ar & numMaterials;

	materials_ = std::map< std::string, std::unique_ptr<Material> >();

	for (glmd::uint32 i=0; i < numMaterials; i++)
	{
		auto s = std::string();
		ar & s;
		
		auto material = std::unique_ptr<Material>( new Material(openGlDevice_, s) );
		ar & *(material.get());
		
		materials_[s] = std::move(material);
	}
}

}
}

BOOST_CLASS_EXPORT(glr::glw::IMaterialManager)
BOOST_CLASS_EXPORT_GUID(glr::glw::MaterialManager, "glr::glw::MaterialManager")
