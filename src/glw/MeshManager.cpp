#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "glw/MeshManager.hpp"


namespace glr
{
namespace glw
{

MeshManager::MeshManager()
{
	openGlDevice_ = nullptr;
}
	
MeshManager::MeshManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

MeshManager::~MeshManager()
{
}

Mesh* MeshManager::getMesh(const std::string& name) const
{
	auto it = meshes_.find(name);
	if ( it != meshes_.end() )
	{
		LOG_DEBUG( "Mesh '" + name + "' found." );
		return it->second.get();
	}

	LOG_DEBUG( "Mesh '" + name + "' not found." );
	
	return nullptr;
}

Mesh* MeshManager::addMesh(const std::string& name)
{
	LOG_DEBUG( "Loading mesh..." );

	auto it = meshes_.find(name);
	if ( it != meshes_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Mesh already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Mesh." );
	meshes_[name] = std::unique_ptr<Mesh>(new Mesh(openGlDevice_, name));

	return meshes_[name].get();
}

Mesh* MeshManager::addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData
	)
{
	LOG_DEBUG( "Loading mesh..." );

	auto it = meshes_.find(name);
	if ( it != meshes_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Mesh already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Mesh." );
	meshes_[name] = std::unique_ptr<Mesh>(new Mesh(openGlDevice_, name, vertices, normals, textureCoordinates, colors, bones, boneData));

	return meshes_[name].get();
}

Mesh* MeshManager::addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	)
{
	LOG_DEBUG( "Loading mesh..." );

	auto it = meshes_.find(name);
	if ( it != meshes_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Mesh already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Mesh." );
	meshes_[name] = std::unique_ptr<Mesh>(new Mesh(openGlDevice_, name, vertices, normals, textureCoordinates, colors));

	return meshes_[name].get();
}

void MeshManager::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void MeshManager::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void MeshManager::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void MeshManager::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
}

template<class Archive> void MeshManager::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<MeshManager, IMeshManager>(
		static_cast<MeshManager*>(nullptr),
		static_cast<IMeshManager*>(nullptr)
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

BOOST_CLASS_EXPORT(glr::glw::IMeshManager)
BOOST_CLASS_EXPORT_GUID(glr::glw::MeshManager, "glr::glw::MeshManager")
