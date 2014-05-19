#ifndef IMESHMANAGER_H_
#define IMESHMANAGER_H_

#include <string>

#include "Mesh.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

class Mesh;

class IMeshManager : public virtual serialize::ITextSerializable
{
public:
	virtual ~IMeshManager()
	{
	}
	;

	/**
	 * Returns the mesh with the given name.  If a mesh with the given name doesn't exist, it returns
	 * nullptr.
	 * 
	 * @param name The name of the mesh to retrieve.
	 * 
	 * @return The Mesh object with name 'name', or nullptr if no mesh exists with that name.
	 */
	virtual Mesh* getMesh(const std::string& name) const = 0;
	
	/**
	 * Creates a mesh with the given name.
	 * 
	 * If a mesh already exists with the given name, it will return that mesh.
	 * 
	 * @param name The name to use for the new mesh.
	 * 
	 * @return A Mesh object.
	 */
	virtual Mesh* addMesh(const std::string& name) = 0;
	
	/**
	 * Creates a mesh with the given name and using the provided mesh data.
	 * 
	 * If a mesh already exists with the given name, it will return that mesh.
	 * 
	 * @param name The name to use for the new mesh.
	 * @param vertices
	 * @param normals
	 * @param textureCoordinates
	 * @param colors
	 * @param bones
	 * 
	 * @return A Mesh object.
	 */
	virtual Mesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData
	) = 0;
	
	/**
	 * Creates a mesh with the given name and using the provided mesh data.
	 * 
	 * If a mesh already exists with the given name, it will return that mesh.
	 * 
	 * @param name The name to use for the new mesh.
	 * @param vertices
	 * @param normals
	 * @param textureCoordinates
	 * @param colors
	 * 
	 * @return A Mesh object.
	 */
	virtual Mesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	) = 0;
};

}
}

#endif /* IMESHMANAGER_H_ */
