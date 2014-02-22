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

Model::Model(glw::IMesh* mesh, glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	meshManager_ = openGlDevice_->getMeshManager();
	materialManager_ = openGlDevice_->getMaterialManager();
	textureManager_ = openGlDevice_->getTextureManager();
	animationManager_ = openGlDevice_->getAnimationManager();
	
	meshes_.push_back( mesh );
	textures_.push_back( nullptr );
	materials_.push_back( nullptr );
	
	animations_ = std::map< std::string, std::unique_ptr<Animation>>();
	
	rootBoneNode_ = glw::BoneNode();
	
	currentAnimation_ = nullptr;
	
	// NOTE: the below is prime for a memory leak
	emptyAnimation_ = new glw::Animation( openGlDevice_, "EMPTY" );
	emptyAnimation_->generateIdentityBoneTransforms( 100 );
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
		std::cout << a.first << std::endl;
		animations_[ a.first ] = std::unique_ptr<Animation>( new Animation(*a.second.get()) );
	}
	
	// TODO: Do we want to actually do a copy on this each time?
	rootBoneNode_ = other.rootBoneNode_;
	
	globalInverseTransformation_ = other.globalInverseTransformation_;
	
	currentAnimation_ = nullptr;
	// Set the current animation to our copied animation
	if ( other.currentAnimation_ != nullptr )
	{
		std::cout << other.currentAnimation_->getName() << std::endl;
		auto it = animations_.find( other.currentAnimation_->getName() );
		if (it != animations_.end())
		{
			currentAnimation_ = it->second.get();
		}
	}
	
	// TODO: make this not crappy
	emptyAnimation_ = new glw::Animation( openGlDevice_, "EMPTY" );
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
	
	// Default to no animation
	currentAnimation_ = nullptr;
	
	for ( auto d : modelData)
	{
		auto mesh = meshManager_->getMesh(d->meshData.name);
		if (mesh == nullptr)
			mesh = meshManager_->addMesh(d->meshData.name, d->meshData.vertices, d->meshData.normals, d->meshData.textureCoordinates, d->meshData.colors, d->meshData.bones, d->boneData);
		
		meshes_.push_back( mesh );
		
		
		if ( d->textureData.filename != std::string("") )
		{
			auto texture = textureManager_->getTexture2D(d->textureData.filename);
			if (texture == nullptr)
				texture = textureManager_->addTexture2D(d->textureData.filename, d->textureData.filename, d->textureData.settings);
			
			textures_.push_back( texture );
		}
		else
		{
			textures_.push_back( nullptr );
		}


		auto material = materialManager_->getMaterial(d->materialData.name);
		if (material == nullptr)
			material = materialManager_->addMaterial(d->materialData.name, d->materialData.ambient, d->materialData.diffuse, d->materialData.specular, d->materialData.emission, d->materialData.shininess, d->materialData.strength);
		
		materials_.push_back( material );
		
		// Create bone structure (tree structure)
		rootBoneNode_ = d->animationSet.rootBoneNode;
		
		// Set the global inverse transformation
		globalInverseTransformation_ = d->globalInverseTransformation;
		
		// Load the animation information
		for ( auto& kv : d->animationSet.animations)
		{
			auto animation = animationManager_->getAnimation( kv.first );
			
			if (animation == nullptr)
			{	
				// Create animated bone node information
				auto animatedBoneNodes = std::map< std::string, glw::AnimatedBoneNode >();
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
	emptyAnimation_ = new glw::Animation( openGlDevice_, "EMPTY" );
	emptyAnimation_->generateIdentityBoneTransforms( 100 );
}

void Model::destroy()
{
}

glw::IMesh* Model::getMesh(glmd::uint32 index)
{
	return meshes_[index];
}

void Model::removeMesh(glmd::uint32 index)
{
	meshes_.erase(meshes_.begin() + index);
	materials_.erase(materials_.begin() + index);
	textures_.erase(textures_.begin() + index);
}

void Model::removeMesh(glw::IMesh* mesh)
{
	glmd::int32 index = getIndexOf(mesh);
	
	meshes_.erase(meshes_.begin() + index);
	materials_.erase(materials_.begin() + index);
	textures_.erase(textures_.begin() + index);
}

void Model::addMesh(glw::IMesh* mesh)
{
	assert( mesh != nullptr );
	
	meshes_.push_back(mesh);
	materials_.push_back( nullptr );
	textures_.push_back( nullptr );
}

void Model::addMesh(glw::IMesh* mesh, glmd::uint32 index)
{
	assert( mesh != nullptr );
	
	meshes_.insert(meshes_.begin() + index, mesh);
	materials_.insert(materials_.begin() + index, nullptr);
	textures_.insert(textures_.begin() + index, nullptr);
}

glmd::uint32 Model::getNumberOfMeshes()
{
	return meshes_.size();
}

glw::ITexture* Model::getTexture(glmd::uint32 index)
{
	return textures_[index];
}

glw::ITexture* Model::getTexture(glw::IMesh* mesh)
{
	glmd::int32 i = getIndexOf(mesh);
	if (i >= 0)
		return textures_[i];
	
	return nullptr;
}

void Model::removeTexture(glmd::uint32 index)
{
	textures_[index] = nullptr;
}

void Model::removeTexture(glw::ITexture* texture)
{
	glmd::int32 i = getIndexOf(texture);
	if (i >= 0)
		textures_[i] = nullptr;
}

void Model::removeTexture(glw::IMesh* mesh)
{
	glmd::int32 i = getIndexOf(mesh);
	if (i >= 0)
		textures_[i] = nullptr;
}

void Model::addTexture(glw::ITexture* texture, glmd::uint32 index)
{
	assert( texture != nullptr );
	//assert( index < meshes_.size() );
	
	textures_[index] = texture;
}

void Model::addTexture(glw::ITexture* texture, glw::Mesh* mesh)
{
	glmd::int32 i = getIndexOf(mesh);
	if (i >= 0)
		textures_[i] = texture;
}

glmd::uint32 Model::getNumberOfTextures()
{
	return textures_.size();
}

glw::IMaterial* Model::getMaterial(glmd::uint32 index)
{
	return materials_[index];
}

glw::IMaterial* Model::getMaterial(glw::IMesh* mesh)
{
	glmd::int32 i = getIndexOf(mesh);
	if (i >= 0)
		return materials_[i];
	
	return nullptr;
}

void Model::removeMaterial(glmd::uint32 index)
{
	materials_[index] = nullptr;
}

void Model::removeMaterial(glw::IMaterial* material)
{
	glmd::int32 i = getIndexOf(material);
	if (i >= 0)
		materials_[i] = nullptr;
}

void Model::removeMaterial(glw::IMesh* mesh)
{
	glmd::int32 i = getIndexOf(mesh);
	if (i >= 0)
		materials_[i] = nullptr;
}

void Model::addMaterial(glw::IMaterial* material, glmd::uint32 index)
{
	assert( material != nullptr );
	//assert( index < meshes_.size() );
	
	materials_[index] = material;
}

void Model::addMaterial(glw::IMaterial* material, glw::Mesh* mesh)
{
	glmd::int32 i = getIndexOf(mesh);
	if (i >= 0)
		materials_[i] = material;
}

glmd::uint32 Model::getNumberOfMaterials()
{
	return materials_.size();
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
 * Will return the animation associated with this model with the given name.  Returns nullptr if no animation
 * is associated with this model with the given name.
 * 
 * @param name The name of the animation to retrieve.
 * 
 * @return A pointer to the animation associated with this model with the given name; nullptr if no animation
 * is associated with this model with the given name.
 */
IAnimation* Model::getAnimation(const std::string& name)
{	
	if ( animations_.find(name) != animations_.end() )
	{
		return animations_[name].get();
	}
	
	return nullptr;
}

void Model::removeAnimation(const std::string& name)
{
}

void Model::removeAnimation(glw::IAnimation* animation)
{
}

void Model::addAnimation(glw::IAnimation* animation)
{
}

glmd::uint32 Model::getNumberOfAnimations()
{
	return animations_.size();
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

glmd::int32 Model::getIndexOf(glw::IMesh* mesh)
{
	for (glmd::uint32 i=0; i < meshes_.size(); i++)
	{
		if (meshes_[i] == mesh)
			return i;
	}
	
	return -1;
}

glmd::int32 Model::getIndexOf(glw::ITexture* texture)
{
	for (glmd::uint32 i=0; i < textures_.size(); i++)
	{
		if (textures_[i] == texture)
			return i;
	}
	
	return -1;
}

glmd::int32 Model::getIndexOf(glw::IMaterial* material)
{
	for (glmd::uint32 i=0; i < materials_.size(); i++)
	{
		if (materials_[i] == material)
			return i;
	}
	
	return -1;
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
	assert(shader != nullptr);
	
	for ( glm::detail::uint32 i = 0; i < meshes_.size(); i++ )
	{
		if ( textures_[i] != nullptr )
		{
			// TODO: bind to an actual texture position (for multiple textures per mesh, which we currently don't support...maybe at some point we will???  Why would we need multiple textures?)
			//shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_TEXTURE_2D, textures_[i]->getBindPoint() );
			textures_[i]->bind();
			/*
			GLint bindPoint = shader->getBindPointByBindingName( shaders::IShader::BIND_TYPE_TEXTURE_2D );
			if (bindPoint >= 0)
			{
				textures_[i]->bind();

				openGlDevice_->bindBuffer( textures_[i]->getBufferId(), bindPoint );
			}
			*/
		}
		
		if ( materials_[i] != nullptr )
		{
			GLint bindPoint = shader->getBindPointByBindingName( shaders::IShader::BIND_TYPE_MATERIAL );
			if (bindPoint >= 0)
			{
				materials_[i]->bind();

				openGlDevice_->bindBuffer( materials_[i]->getBufferId(), bindPoint );
			}
		}		
		
		if (currentAnimation_ != nullptr)
		{
			GLint bindPoint = shader->getBindPointByBindingName( shaders::IShader::BIND_TYPE_BONE );
			if (bindPoint >= 0)
			{
				glw::Animation* a = currentAnimation_->getAnimation();
				a->setAnimationTime( currentAnimation_->getAnimationTime() );
				a->setFrameClampping( currentAnimation_->getStartFrame(), currentAnimation_->getEndFrame() );
				a->generateBoneTransforms(globalInverseTransformation_, rootBoneNode_, meshes_[i]->getBoneData(), currentAnimation_->getIndexCache());
				a->bind();
				
				openGlDevice_->bindBuffer( a->getBufferId(), bindPoint );
			}
		}
		else
		{
			// Zero out the animation data
			// TODO: Do we need to do this?
			// TODO: find a better way to load 'empty' bone data in the shader
			GLint bindPoint = shader->getBindPointByBindingName( shaders::IShader::BIND_TYPE_BONE );
			if (bindPoint >= 0)
			{
				emptyAnimation_->bind();

				openGlDevice_->bindBuffer( emptyAnimation_->getBufferId(), bindPoint );
			}
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


