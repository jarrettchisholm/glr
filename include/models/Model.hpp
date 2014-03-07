#ifndef MODEL_H_
#define MODEL_H_

#include <memory>
#include <map>
#include <string>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "models/IModel.hpp"
#include "models/Animation.hpp"

#include "Id.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "glw/IMaterialManager.hpp"
#include "glw/ITextureManager.hpp"
#include "glw/IMeshManager.hpp"
#include "glw/IAnimationManager.hpp"

#include "glw/IMesh.hpp"
#include "glw/ITexture.hpp"
#include "glw/IMaterial.hpp"
#include "glw/IAnimation.hpp"

namespace glr
{
namespace models
{

class Model : public IModel
{
public:
	Model(Id id, std::string name, glw::IMesh* meshe, glw::ITexture* texture, glw::IMaterial* material, std::vector<glw::IAnimation*> animations, glw::BoneNode rootBoneNode, glm::mat4 globalInverseTransformation, glw::IOpenGlDevice* openGlDevice);
	Model(Id id, std::string name, std::vector<glw::IMesh*> meshes, std::vector<glw::ITexture*> textures, std::vector<glw::IMaterial*> materials, std::vector<glw::IAnimation*> animations, glw::BoneNode rootBoneNode, glm::mat4 globalInverseTransformation, glw::IOpenGlDevice* openGlDevice);
	Model(Id id, const Model& other);
	virtual ~Model();

	glw::IMesh* getMesh(glmd::uint32 index);
	void removeMesh(glmd::uint32 index);
	void removeMesh(glw::IMesh* mesh);
	void addMesh(glw::IMesh* mesh);
	void addMesh(glw::IMesh* mesh, glmd::uint32 index);
	glmd::uint32 getNumberOfMeshes();
	
	glw::ITexture* getTexture(glmd::uint32 index);
	glw::ITexture* getTexture(glw::IMesh* mesh);
	void removeTexture(glmd::uint32 index);
	void removeTexture(glw::ITexture* texture);
	void removeTexture(glw::IMesh* mesh);
	void addTexture(glw::ITexture* texture, glmd::uint32 index);
	void addTexture(glw::ITexture* texture, glw::Mesh* mesh);
	glmd::uint32 getNumberOfTextures();
	
	glw::IMaterial* getMaterial(glmd::uint32 index);
	glw::IMaterial* getMaterial(glw::IMesh* mesh);
	void removeMaterial(glmd::uint32 index);
	void removeMaterial(glw::IMaterial* material);
	void removeMaterial(glw::IMesh* mesh);
	void addMaterial(glw::IMaterial* material, glmd::uint32 index);
	void addMaterial(glw::IMaterial* material, glw::Mesh* mesh);
	glmd::uint32 getNumberOfMaterials();
	
	virtual const Id& getId() const;
	virtual const std::string& getName() const;
	
	virtual IAnimation* getCurrentAnimation();
	virtual IAnimation* getAnimation(const std::string& name);
	void removeAnimation(const std::string& name);
	void removeAnimation(glw::IAnimation* animation);
	void addAnimation(glw::IAnimation* animation);
	glmd::uint32 getNumberOfAnimations();
	virtual void setCurrentAnimation(IAnimation* animation);

	virtual void render(shaders::IShaderProgram* shader);

protected:
	Id id_;
	std::string name_;

	glw::IOpenGlDevice* openGlDevice_;

	std::vector<glw::IMesh*> meshes_;
	std::vector<glw::ITexture*> textures_;
	std::vector<glw::IMaterial*> materials_;
	std::map< std::string, std::unique_ptr<Animation>> animations_;
	
	Animation* currentAnimation_;
	glw::Animation* emptyAnimation_;
	
	/* 	All meshes in this model use this bone node tree for animations.
		Any animations that manipulate bone nodes will be manipulating bones in this bone node tree. */
	glw::BoneNode rootBoneNode_;
	
	glm::mat4 globalInverseTransformation_;
	
	glw::IMeshManager* meshManager_;
	glw::IMaterialManager* materialManager_;
	glw::ITextureManager* textureManager_;
	glw::IAnimationManager* animationManager_;

	void loadMeshes(const aiScene* scene);
	void loadTextures(const aiScene* scene);
	void loadMaterials(const aiScene* scene);
	void loadAnimations(const aiScene* scene);
	
	glmd::int32 getIndexOf(glw::IMesh* mesh);
	glmd::int32 getIndexOf(glw::ITexture* texture);
	glmd::int32 getIndexOf(glw::IMaterial* material);
	
private:
	// We don't copy straight up, since we need a new id for the copy
	Model(const Model& other);
	
	void initialize();
	void destroy();
	
	void copy(const Model& other);

};

}
}

#endif /* MODEL_H_ */
