#ifndef MESHMANAGER_H_
#define MESHMANAGER_H_

#include <memory>
#include <string>
#include <map>
#include <mutex>

#include "IMeshManager.hpp"

#include "IOpenGlDevice.hpp"

#include "Mesh.hpp"

#include "serialize/SplitMember.hpp"

namespace glr
{
namespace glw
{
	
class MeshManager : public IMeshManager
{
public:
	MeshManager(IOpenGlDevice* openGlDevice);
	virtual ~MeshManager();

	virtual IMesh* getMesh(const std::string& name) const;
	virtual IMesh* addMesh(const std::string& name);
	virtual IMesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData,
		bool initialize = true
	);
	virtual IMesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		bool initialize = true
	);
	virtual void destroyMesh( const std::string& name );
	virtual void destroyMesh( IMesh* mesh );
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);
	
private:
	/**
	 * Required by serialization.
	 */
	MeshManager();

	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Mesh> > meshes_;
	
	mutable std::mutex accessMutex_;
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
	// Need to do these because boost serialization doesn't have a standard implementation for std::unique_ptr
	// Apparently, std::unique_ptr will have a serializable implementation in boost 1.56
	// TODO: Implement one myself?
	template<class Archive> void save(Archive & ar, const unsigned int version) const;
	template<class Archive> void load(Archive & ar, const unsigned int version);
};

}
}

#endif /* MESHMANAGER_H_ */
