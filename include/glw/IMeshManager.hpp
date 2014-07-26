#ifndef IMESHMANAGER_H_
#define IMESHMANAGER_H_

#include <string>

#include "Mesh.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

class IMesh;

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
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param name The name of the mesh to retrieve.
	 * 
	 * @return The Mesh object with name 'name', or nullptr if no mesh exists with that name.
	 */
	virtual IMesh* getMesh(const std::string& name) const = 0;
	
	/**
	 * Creates a mesh with the given name.
	 * 
	 * If a mesh already exists with the given name, it will return that mesh.
	 * 
	 * **Partially Thread Safe**: If initialize is false, this method is safe to call in a multi-threaded environment.  However, 
	 * if initialize is true, this method is *not* thread safe, and should only be called from the OpenGL thread.
	 * 
	 * @param name The name to use for the new mesh.
	 * 
	 * @return A Mesh object.
	 */
	virtual IMesh* addMesh(const std::string& name) = 0;
	
	/**
	 * Creates a mesh with the given name and using the provided mesh data.
	 * 
	 * If a mesh already exists with the given name, it will return that mesh.
	 * 
	 * **Partially Thread Safe**: If initialize is false, this method is safe to call in a multi-threaded environment.  However, 
	 * if initialize is true, this method is *not* thread safe, and should only be called from the OpenGL thread.
	 * 
	 * @param name The name to use for the new mesh.
	 * @param vertices
	 * @param normals
	 * @param textureCoordinates
	 * @param colors
	 * @param bones
	 * @param initialize If true, will initialize all of the resources required for this mesh.  Otherwise, it will
	 * just create the mesh and return it (without initializing it).
	 * 
	 * @return A Mesh object.
	 */
	virtual IMesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData,
		bool initialize = true
	) = 0;
	
	/**
	 * Creates a mesh with the given name and using the provided mesh data.
	 * 
	 * If a mesh already exists with the given name, it will return that mesh.
	 * 
	 * **Partially Thread Safe**: If initialize is false, this method is safe to call in a multi-threaded environment.  However, 
	 * if initialize is true, this method is *not* thread safe, and should only be called from the OpenGL thread.
	 * 
	 * @param name The name to use for the new mesh.
	 * @param vertices
	 * @param normals
	 * @param textureCoordinates
	 * @param colors
	 * 
	 * @return A Mesh object.
	 */
	virtual IMesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		bool initialize = true
	) = 0;
	
	/**
	 * Destroys the mesh with the given name.
	 * 
	 * If no mesh exists with the given name, this method will do nothing.
	 * 
	 * Note that once destroyed, all references and pointers to that mesh will be invalid.
	 * 
	 * **Partially Thread Safe**: If the mesh being destroyed has not yet been initialized, this method is safe to call in a multi-threaded environment.  However, 
	 * if it has been initialized, this method is *not* thread safe, and should only be called from the OpenGL thread.
	 * 
	 * @param name The name of the mesh to destroy.
	 */
	virtual void destroyMesh( const std::string& name ) = 0;
	
	/**
	 * Destroys the given mesh.
	 * 
	 * If the mesh is null, an exception will be thrown.
	 * 
	 * Note that once destroyed, all references and pointers to that mesh will be invalid.
	 * 
	 * **Partially Thread Safe**: If the mesh being destroyed has not yet been initialized, this method is safe to call in a multi-threaded environment.  However, 
	 * if it has been initialized, this method is *not* thread safe, and should only be called from the OpenGL thread.
	 * 
	 * @param mesh The mesh to destroy.
	 */
	virtual void destroyMesh( IMesh* mesh ) = 0;
};

}
}

#endif /* IMESHMANAGER_H_ */
