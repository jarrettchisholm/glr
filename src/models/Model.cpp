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

/**
 * Copy constructor.
 */
Model::Model(const Model& other)
{
	openGlDevice_ = other.openGlDevice_;
	
	meshManager_ = openGlDevice_->getMeshManager();
	materialManager_ = openGlDevice_->getMaterialManager();
	textureManager_ = openGlDevice_->getTextureManager();
	animationManager_ = openGlDevice_->getAnimationManager();
	
	meshes_ = other.meshes_;
	textures_ = other.textures_;
	materials_ = other.materials_;
	//animations_ = other.animations_;
	
	animations_ = std::map< std::string, std::unique_ptr<Animation>>();
	
	for ( auto& a : other.animations_)
	{
		animations_[ a.first ] = std::unique_ptr<Animation>( new Animation(*a.second.get()) );
	}
	
	// TODO: Do we want to actually do a copy on this each time?
	rootBoneNode_ = other.rootBoneNode_;
	
	globalInverseTransformation_ = other.globalInverseTransformation_;
	
	currentAnimation_ = other.currentAnimation_;
	
	// TODO: make this not crappy
	emptyAnimation_ = new glw::Animation( openGlDevice_ );
	emptyAnimation_->generateIdentityBoneTransforms( 100 );
}

Model::~Model()
{
}

/**
 * Will initialize this mesh with data given through the modelData object.
 * 
 * @param modelData The data to use to initialize this model.  Each element of the modelData vector corresponds to a new
 * mesh/material/texture/animation set.
 */
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
		// Default to no animation
		currentAnimation_ = nullptr;

		// Create bone structure (tree structure)
		rootBoneNode_ = d->animationSet.rootBoneNode;
		
		// Set the global inverse transformation
		globalInverseTransformation_ = d->globalInverseTransformation;
		
		// Load the animation information
		for ( auto& kv : d->animationSet.animations)
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
			
			assert(animation != nullptr);
			
			animations_[ animation->getName() ] = std::unique_ptr<Animation>( new Animation(animation, openGlDevice_) );
			
			// TODO: add animations properly (i.e. with names specifying the animation i guess?)
			std::cout << "anim: " << animation->getName() << std::endl;
			//if (currentAnimation_ == nullptr)
			currentAnimation_ = animations_[ animation->getName() ].get();
		}
	}
	
	// TODO: make this not crappy
	emptyAnimation_ = new glw::Animation( openGlDevice_ );
	emptyAnimation_->generateIdentityBoneTransforms( 100 );
}

void Model::destroy()
{
}

/**
 * Gets the animation that is currently active for this model.  If no animation is active, it will return
 * nullptr.
 * 
 * @return A pointer to the current IAnimation object, or nullptr if there is no current animation
 */
IAnimation* Model::getCurrentAnimation()
{
	return currentAnimation_;
}

/**
 * Sets the current animation.
 * 
 * @param animation A pointer to the animation to use.  If nullptr is sent in, the current animation is set to nullptr (i.e. no current
 * animation)
 */
void Model::setCurrentAnimation(IAnimation* animation)
{
	currentAnimation_ = static_cast<Animation*>(animation);
}

/**
 * Will return the animation associated with this model with the given name.  Returns nullptr if no animation
 * is associated with this model with the given name.
 * 
 * @param name The name of the animation to retrieve.
 * 
 * @return A pointer to the animation associated with this model with the given name; nullptr if no animation
 * is associated with this model with the given name.
 */
IAnimation* Model::getAnimation(const std::string name)
{	
	if ( animations_.find(name) != animations_.end() )
	{
		return animations_[name].get();
	}
	
	return nullptr;
}

/**
 * Will render the model through OpenGL.
 * 
 * The model will render each mesh individually.  Each mesh may have a corresponding material, texture, and animation, all of which
 * will be bound to the shader for use when rendering the mesh.
 * 
 * @param shader The shader to use to render this model.
 */
void Model::render(shaders::IShaderProgram* shader)
{
	for ( glm::detail::uint32 i = 0; i < meshes_.size(); i++ )
	{
		if ( textures_[i] != nullptr )
		{
			// TODO: bind to an actual texture position (for multiple textures per mesh, which we currently don't support...maybe at some point we will???  Why would we need multiple textures?)
			textures_[i]->bind();
			//shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_TEXTURE, textures_[i]->getBindPoint() );
		}
		
		if ( materials_[i] != nullptr )
		{
			materials_[i]->bind();
			shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_MATERIAL, materials_[i]->getBindPoint() );
		}		
		
		if (currentAnimation_ != nullptr)
		{			
			glw::Animation* a = currentAnimation_->getAnimation();
			a->setAnimationTime( currentAnimation_->getAnimationTime() );
			a->generateBoneTransforms(globalInverseTransformation_, rootBoneNode_, meshes_[i]->getBoneData());
			a->bind();
			shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_BONE, a->getBindPoint() );
		}
		else
		{
			// Zero out the animation data
			// TODO: Do we need to do this?
			// TODO: find a better way to load 'empty' bone data in the shader
			emptyAnimation_->bind();
			shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_BONE, emptyAnimation_->getBindPoint() );
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


