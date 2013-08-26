/*
 * Model.cpp
 *
 *  Created on: 2012-09-20
 *      Author: jarrett
 */

#include "Model.h"

#include "../glw/TextureManager.h"

namespace glr {
namespace models {
Model::Model(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	initialize();
}

Model::Model(std::vector< std::shared_ptr<ModelData> > modelData, glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	initialize(modelData);
}

Model::~Model()
{
}

void Model::initialize(std::vector< std::shared_ptr<ModelData> > modelData)
{
	meshManager_ = openGlDevice_->getMeshManager();
	materialManager_ = openGlDevice_->getMaterialManager();
	textureManager_ = openGlDevice_->getTextureManager();
	animationManager_ = openGlDevice_->getAnimationManager();
	
	for ( std::shared_ptr<ModelData> d : modelData)
	{
		glw::Mesh* mesh = meshManager_->getMesh(d->meshData.name);
		if (mesh == nullptr)
			mesh = meshManager_->addMesh(d->meshData.name, d->meshData.vertices, d->meshData.normals, d->meshData.textureCoordinates, d->meshData.colors, d->meshData.bones);
		
		meshes_.push_back( mesh );
		
		
		glw::Texture* texture = textureManager_->getTexture(d->textureData.filename);
		if (texture == nullptr)
			texture = textureManager_->addTexture(d->textureData.filename);
		
		textures_.push_back( texture );


		glw::Material* material = materialManager_->getMaterial(d->materialData.name);
		if (material == nullptr)
			material = materialManager_->addMaterial(d->materialData.name, d->materialData.ambient, d->materialData.diffuse, d->materialData.specular, d->materialData.emission, d->materialData.shininess, d->materialData.strength);
		
		materials_.push_back( material );
		
		
/*
struct BoneNode {
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

struct AnimatedBoneNode {
	std::string name;
	std::vector< glm::detail::float64 > positionTimes;
	std::vector< glm::detail::float64 > rotationTimes;
	std::vector< glm::detail::float64 > scalingTimes;
	std::vector< glm::vec3 > positions;
	std::vector< glm::quat > rotations;
	std::vector< glm::vec3 > scalings;
};

struct Animation {
	std::string name;
	glm::detail::float64 duration;
	glm::detail::float64 ticksPerSecond;
	
	std::map< std::string, AnimatedBoneNode > animatedBoneNodes;
};

struct AnimationData {
	std::string name;
	
	BoneNode rootBoneNode;
	std::map< std::string, Animation > animations;
};
*/
		for ( auto& kv : d->animationData.animations)
		{
			glw::Animation* animation = animationManager_->getAnimation( kv.first );
			
			if (animation == nullptr)
			{
				// TODO: create bone structure (tree structure)
				
				// TODO: create animated bone node information
				
				// TODO: actually create the animation
				animation = animationManager_->addAnimation( kv.first );
			}			
			
			animations_.push_back( animation );
		}
	}
}

void Model::destroy()
{
}

void Model::render(shaders::IShaderProgram* shader)
{
	for ( glm::detail::uint32 i = 0; i < meshes_.size(); i++ )
	{
		if ( textures_[i] != nullptr )
		{
			// TODO: bind to an actual texture position (for multiple textures per mesh, which we currently don't support...maybe at some point we will???  Why would we need multiple textures?)
			textures_[i]->bind();
			shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_TEXTURE, textures_[i]->getBindPoint() );
		}
		
		if ( materials_[i] != nullptr )
		{
			materials_[i]->bind();
			shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_MATERIAL, materials_[i]->getBindPoint() );
		}
		
		meshes_[i]->render();
	}
}

}
}


