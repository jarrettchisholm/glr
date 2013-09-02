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
			mesh = meshManager_->addMesh(d->meshData.name, d->meshData.vertices, d->meshData.normals, d->meshData.textureCoordinates, d->meshData.colors, d->meshData.bones, d->boneData);
		
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

		// Create bone structure (tree structure)
		rootBoneNode_ = d->animationData.rootBoneNode;
		
		// Set the global inverse transformation
		globalInverseTransformation_ = d->globalInverseTransformation;
		
		// Load the animation information
		for ( auto& kv : d->animationData.animations)
		{
			glw::Animation* animation = animationManager_->getAnimation( kv.first );
			
			if (animation == nullptr)
			{	
				// Create animated bone node information
				std::map< std::string, glw::AnimatedBoneNode > animatedBoneNodes = std::map< std::string, glw::AnimatedBoneNode >();
				for ( auto& kvAnimatedBoneNode : kv.second.animatedBoneNodes )
				{
					animatedBoneNodes[ kvAnimatedBoneNode.first ] = glw::AnimatedBoneNode( 
						kvAnimatedBoneNode.second.name, 
						kvAnimatedBoneNode.second.positionTimes, 
						kvAnimatedBoneNode.second.rotationTimes, 
						kvAnimatedBoneNode.second.scalingTimes, 
						kvAnimatedBoneNode.second.positions, 
						kvAnimatedBoneNode.second.rotations, 
						kvAnimatedBoneNode.second.scalings
					);
				}
				
				// Actually create the animation
				animation = animationManager_->addAnimation( kv.second.name, kv.second.duration, kv.second.ticksPerSecond, animatedBoneNodes );
			}			
			
			// TODO: add animations properly (i.e. with names specifying the animation i guess?)
			animation_ = animation;
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
		
		if (animation_ != nullptr)
		{
			// TODO: is this the best place for calling the tick() method?
			//animation_->tick(0.0f);
			animation_->generateBoneTransforms(0.25f, globalInverseTransformation_, rootBoneNode_, meshes_[i]->getBoneData());
			animation_->bind();
			shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_BONE, animation_->getBindPoint() );
		}
		
		meshes_[i]->render();
	}
}

/**
 * Helper method - will take the data in the BoneNode structure and create a glw::BoneNode object with it.
 * 
 * NOTE: I'm not sure this is the best place for this...
 * 
 * @param n The BoneNode structure to copy.
 * 
 * @return The glw::BoneNode object that contains a copy of the data in the BoneNode structure.
 */
/*
glw::BoneNode Model::copyBoneNode( BoneNode& n )
{
	glw::BoneNode n = glw::BoneNode( n.rootBoneNode.name, n.rootBoneNode.transformation );
	
	for ( auto& child : n.rootBoneNode.children )
	{
		n.addChild( copyBoneNode( child ) );
	}
	
	return n;
}
*/

}
}


