#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <string>
#include <vector>
#include <memory>

#include <assimp/cimport.h>
#include <assimp/scene.h>

#include "glw/IOpenGlDevice.hpp"

#include "models/Model.hpp"

#include "IdManager.hpp"

namespace glmd = glm::detail;

namespace glr
{
namespace models
{

class ModelLoader
{
public:
	ModelLoader(glw::IOpenGlDevice* openGlDevice);
	virtual ~ModelLoader();

	std::unique_ptr<Model> loadModel(const std::string& name, const std::string& filename, IdManager& idManager);
	
	/**
	 * Loads model data from the file specified by filename.
	 * 
	 * @param filename The file we want to load
	 * @param idManager
	 * 
	 * @returns A vector of ModelData objects (as shared pointers).  We do this so as not to have to copy the
	 * model data when we return from this method.
	 */
	std::unique_ptr<Model> loadModel(const std::string& filename, IdManager& idManager);
	
	/**
	 * 
	 */
	std::pair<std::vector< ModelData >, AnimationSet> loadModelData(const std::string& name, const std::string& filename);
	
private:
	aiLogStream stream;
	
	glw::IOpenGlDevice* openGlDevice_;
	
	/**
	 * Will initialize a model with data given through the modelData object.
	 * 
	 * @param modelData The data to use to initialize this model.  Each element of the modelData vector corresponds to a new
	 * mesh/material/texture/animation set.
	 * @param animationSet The set of animations to be associated with the model.
	 * @param idManager
	 */
	std::unique_ptr<Model> generateModel(const std::string& name, const std::vector<ModelData>& modelData, const AnimationSet& animationSet, IdManager& idManager);

	/**
	 * Load the vertex, normal, texture, and color data for the given mesh.
	 * 
	 * Note: We only support models that have faces with 3 points (i.e. triangles)
	 * 
	 * @param filename The file being loaded
	 * @param index The current index of the mesh being loaded
	 * @param mesh The AssImp mesh data structure to load data from.  Must not be null.
	 * @param boneIndexMap A map associating an AssImp bone name with a bone index (of type uint32).  Note that this variable is NOT const - this is
	 * only so that we can access elements in the map using the operator[] operator.
	 */
	MeshData loadMesh(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMesh* mesh, std::map< std::string, glmd::uint32 >& boneIndexMap);
	
	/**
	 * Load the texture filename for the given material.
	 * 
	 * @param filename The file being loaded
	 * @param index The current index of the texture being loaded
	 * @param mesh The AssImp material data structure to load data from.  Must not be null.
	 */
	TextureData loadTexture(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMaterial* material);
	
	/**
	 * Load the material data for the given material.
	 * 
	 * @param filename The file being loaded
	 * @param index The current index of the material being loaded
	 * @param material The AssImp material data structure to load data from.  Must not be null.
	 */
	MaterialData loadMaterial(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMaterial* material);
	
	/**
	 * Will load the bone data for the given AssImp mesh.
	 * 
	 * @param filename The file being loaded
	 * @param index The current index of the mesh being loaded
	 * @param mesh The AssImp mesh data structure to load data from.  Must not be null.
	 */
	glw::BoneData loadBones(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMesh* mesh);
	
	/**
	 * Will load the animations for the given scene.
	 * 
	 * @param filename The file being loaded
	 * @param scene The AssImp scene data structure to load animation data from.  Must not be null.
	 */
	AnimationSet loadAnimations(const std::string& name, const std::string& filename, const aiScene* scene);
	
	/**
	 * Helper method - recursively loads bone node information from the AssImp node.
	 * 
	 * @param node An AssImp Node.
	 * 
	 * @return The BoneNode containing the bone information from the given AssImp node.
	 */
	glw::BoneNode loadBoneNode( const aiNode* node );
	
	/**
	 * Helper method - converts an AssImp 4x4 matrix into a glm 4x4 matrix.
	 * 
	 * @param m A 4x4 AssImp matrix
	 * 
	 * @return A 4x4 glm matrix
	 */
	glm::mat4 convertAssImpMatrix(const aiMatrix4x4* m);
};

}
}

#endif /* MODELLOADER_H_ */
