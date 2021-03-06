#include <utility>

#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "exceptions/InvalidArgumentException.hpp"
#include "common/logger/Logger.hpp"

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

IMesh* MeshManager::getMesh(const std::string& name) const
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	auto it = meshes_.find(name);
	if ( it != meshes_.end() )
	{
		LOG_DEBUG( "Mesh '" + name + "' found." );
		return it->second.get();
	}

	LOG_DEBUG( "Mesh '" + name + "' not found." );
	
	return nullptr;
}

IMesh* MeshManager::addMesh(const std::string& name)
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Loading mesh..." );
	
	auto it = meshes_.find(name);
	if ( it != meshes_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Mesh already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Mesh." );
	auto mesh = std::unique_ptr<Mesh>(new Mesh(openGlDevice_, name));
	auto meshPointer = mesh.get();
	
	meshes_[name] = std::move(mesh);

	return meshPointer;
}

IMesh* MeshManager::addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData,
		bool initialize
	)
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Loading mesh..." );
	
	auto it = meshes_.find(name);
	if ( it != meshes_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Mesh already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Mesh." );
	auto mesh = std::unique_ptr<Mesh>(new Mesh(openGlDevice_, name, vertices, normals, textureCoordinates, colors, bones, boneData, initialize));
	auto meshPointer = mesh.get();
	
	meshes_[name] = std::move(mesh);

	return meshPointer;
}

IMesh* MeshManager::addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		bool initialize
	)
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Loading mesh..." );
	
	auto it = meshes_.find(name);
	if ( it != meshes_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Mesh already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Mesh." );
	auto mesh = std::unique_ptr<Mesh>(new Mesh(openGlDevice_, name, vertices, normals, textureCoordinates, colors));
	auto meshPointer = mesh.get();
	
	meshes_[name] = std::move(mesh);

	return meshPointer;
}

void MeshManager::destroyMesh( const std::string& name )
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Destroying mesh..." );
	
	auto it = meshes_.find(name);
	if ( it != meshes_.end() )
	{
		meshes_.erase( it );
	}
	else
	{
		LOG_DEBUG( "Mesh does not exist." );
	}
}

void MeshManager::destroyMesh( IMesh* mesh )
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Destroying mesh..." );
	
	if (mesh == nullptr)
	{
		std::string msg = std::string("Mesh pointer cannot be null.");
		LOG_ERROR(msg);
		throw exception::InvalidArgumentException(msg);
	}
	
	const auto findFunction = [mesh](const std::map<std::string, std::unique_ptr<Mesh>>::value_type& node) { return node.second.get() == mesh; };
	auto it = std::find_if(meshes_.begin(), meshes_.end(), findFunction);
	
	if ( it != meshes_.end() )
	{
		meshes_.erase( it );
	}
	else
	{
		LOG_DEBUG( "Mesh does not exist." );
	}
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
	boost::serialization::split_member(ar, *this, version);
}

template<class Archive> void MeshManager::save(Archive& ar, const unsigned int version) const
{
	boost::serialization::void_cast_register<MeshManager, IMeshManager>(
		static_cast<MeshManager*>(nullptr),
		static_cast<IMeshManager*>(nullptr)
	);
	
	auto size = meshes_.size();
	ar & size;
	for (auto& it : meshes_)
	{
		ar & it.first & *(it.second.get());
	}
}

template<class Archive> void MeshManager::load(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<MeshManager, IMeshManager>(
		static_cast<MeshManager*>(nullptr),
		static_cast<IMeshManager*>(nullptr)
	);

	std::map< std::string, std::unique_ptr<Mesh> >::size_type numMeshes = 0;
	ar & numMeshes;

	meshes_ = std::map< std::string, std::unique_ptr<Mesh> >();

	for (glmd::uint32 i=0; i < numMeshes; i++)
	{
		auto s = std::string();
		ar & s;
		
		auto mesh = std::unique_ptr<Mesh>( new Mesh(openGlDevice_, s) );
		ar & *(mesh.get());
		
		meshes_[s] = std::move(mesh);
	}
}

}
}

BOOST_CLASS_EXPORT(glr::glw::IMeshManager)
BOOST_CLASS_EXPORT_GUID(glr::glw::MeshManager, "glr::glw::MeshManager")
