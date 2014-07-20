#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include <iostream>
#include <fstream>

// C++ importer interface
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "glw/IMaterialManager.hpp"
#include "glw/ITextureManager.hpp"
#include "glw/IMeshManager.hpp"
#include "glw/IAnimationManager.hpp"

#include "glw/IMesh.hpp"
#include "glw/ITexture.hpp"
#include "glw/IMaterial.hpp"
#include "glw/IAnimation.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "glw/BoneNode.hpp"
#include "glw/BoneData.hpp"
#include "glw/AnimatedBoneNode.hpp"
#include "models/ModelLoader.hpp"
#include "models/Model.hpp"
#include "models/AnimationSet.hpp"
#include "models/AnimationData.hpp"
#include "models/ModelData.hpp"
#include "models/MeshData.hpp"
#include "models/TextureData.hpp"
#include "models/MaterialData.hpp"

#include "common/utilities/AssImpUtilities.hpp"
#include "exceptions/GlException.hpp"

namespace glr
{
namespace models
{

ModelLoader::ModelLoader(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
#ifdef DEBUG
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, nullptr);
	aiAttachLogStream(&stream);
#endif
}

ModelLoader::~ModelLoader()
{
	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
#ifdef DEBUG
	aiDetachAllLogStreams();
#endif
}

std::unique_ptr<Model> ModelLoader::loadModel(const std::string& filename, IdManager& idManager)
{
	return loadModel(filename, filename, idManager);
}

std::unique_ptr<Model> ModelLoader::loadModel(const std::string& name, const std::string& filename, IdManager& idManager)
{
	LOG_DEBUG( "Loading model '" << name << "' - " << filename << "." );

	auto modelData = loadModelData(name, filename);

	LOG_DEBUG( "Done loading model '" << name << "'." );

	return generateModel(name, modelData.first, modelData.second, idManager);
}

std::pair<std::vector< ModelData >, AnimationSet> ModelLoader::loadModelData(const std::string& name, const std::string& filename)
{
	LOG_DEBUG( "Loading model data from file '" << filename << "'." );

	auto modelData = std::vector< ModelData >();

	// We don't currently support aiProcess_JoinIdenticalVertices or aiProcess_FindInvalidData
	// aiProcess_JoinIdenticalVertices - doesn't work because we don't use vertex index lists (maybe we should?)
	// aiProcess_FindInvalidData - I think it's due to the reduction of animation tracks containing redundant keys..
	const aiScene* scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality ^ aiProcess_JoinIdenticalVertices ^ aiProcess_FindInvalidData);

	// Error checking
	if ( scene == nullptr )
	{		
		std::string msg = std::string("Unable to load model...");
		LOG_ERROR( msg );
		throw exception::Exception(msg);
	}
	else if ( scene->HasTextures() )
	{
		// Cleanup
		aiReleaseImport(scene);
		
		std::string msg = std::string("Support for meshes with embedded textures is not implemented.");
		LOG_ERROR( msg );
		throw exception::Exception(msg);
	}
	
	modelData.resize( scene->mNumMeshes );
	
	AnimationSet animationSet = loadAnimations(name, filename, scene);
	
	std::stringstream msg;
	msg << "Model has " << modelData.size() << " meshes.";
	LOG_DEBUG( msg.str() );
	
	for ( glmd::uint32 i=0; i < modelData.size(); i++ )
	{
		modelData[i] = ModelData();
		
		modelData[i].boneData = loadBones( name, filename, i, scene->mMeshes[i] );
		modelData[i].meshData = loadMesh( name, filename, i, scene->mMeshes[i], modelData[i].boneData.boneIndexMap );
		modelData[i].textureData = loadTexture( name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ] );
		modelData[i].materialData = loadMaterial( name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ] );
	}
	

	// cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	// TODO: Should I use raw pointer instead of wrapping it in shared_ptr???
	aiReleaseImport(scene);

	LOG_DEBUG( "Done loading model data for file '" << filename << "'." );
	
	return std::pair<std::vector< ModelData >, AnimationSet>( std::move(modelData), std::move(animationSet) );
}

std::unique_ptr<Model> ModelLoader::generateModel(const std::string& name, const std::vector<ModelData>& modelData, const AnimationSet& animationSet, IdManager& idManager)
{
	auto meshManager = openGlDevice_->getMeshManager();
	auto materialManager = openGlDevice_->getMaterialManager();
	auto textureManager = openGlDevice_->getTextureManager();
	auto animationManager = openGlDevice_->getAnimationManager();
	
	auto meshes = std::vector<glw::IMesh*>();
	auto textures = std::vector<glw::ITexture*>();
	auto materials = std::vector<glw::IMaterial*>();
	auto animations = std::vector<glw::IAnimation*>();
	
	for ( auto& d : modelData)
	{
		auto mesh = meshManager->getMesh(d.meshData.name);
		if (mesh == nullptr)
			mesh = meshManager->addMesh(d.meshData.name, d.meshData.vertices, d.meshData.normals, d.meshData.textureCoordinates, d.meshData.colors, d.meshData.bones, d.boneData);
		
		meshes.push_back( mesh );
		
		
		if ( !d.textureData.filename.empty() )
		{
			auto texture = textureManager->getTexture2D(d.textureData.filename);
			if (texture == nullptr)
				texture = textureManager->addTexture2D(d.textureData.filename, d.textureData.filename, d.textureData.settings);
			
			textures.push_back( texture );
		}
		else
		{
			textures.push_back( nullptr );
		}


		auto material = materialManager->getMaterial(d.materialData.name);
		if (material == nullptr)
			material = materialManager->addMaterial(d.materialData.name, d.materialData.ambient, d.materialData.diffuse, d.materialData.specular, d.materialData.emission, d.materialData.shininess, d.materialData.strength);
		
		materials.push_back( material );
	}
	
	// Create bone structure (tree structure)
	auto rootBoneNode = animationSet.rootBoneNode;
	
	// Set the global inverse transformation
	auto globalInverseTransformation = animationSet.globalInverseTransformation;
	
	// Load the animation information
	for ( auto& kv : animationSet.animations)
	{
		auto animation = animationManager->getAnimation( kv.first );
		
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
			animation = animationManager->addAnimation( kv.second.name, kv.second.duration, kv.second.ticksPerSecond, animatedBoneNodes );
		}
		
		assert(animation != nullptr);
		
		animations.push_back(animation);
		
		// TODO: add animations properly (i.e. with names specifying the animation i guess?)
		std::cout << "anim: " << animation->getName() << std::endl;
	}
	
	std::unique_ptr<Model> model = std::unique_ptr<Model>( new Model(idManager.createId(), name, meshes, textures, materials, animations, rootBoneNode, globalInverseTransformation, openGlDevice_) );
	
	return std::move(model);
}

MeshData ModelLoader::loadMesh(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMesh* mesh, std::map< std::string, glmd::uint32 >& boneIndexMap)
{
	MeshData data = MeshData();
	
	LOG_DEBUG( "loading mesh '" << filename << "'." );
	
	// Set the mesh name
	if (mesh->mName.length > 0)
		data.name = std::string( mesh->mName.C_Str() );
	else
		data.name = name + "_mesh_" + std::to_string(index);
	LOG_DEBUG( "mesh name: " << data.name );

	// Load vertices, normals, texture coordinates, and colors
	glm::detail::uint32 currentIndex = 0;
	
	std::vector< glmd::uint32 > vertexIndexMap = std::vector< glmd::uint32 >();
	vertexIndexMap.resize( mesh->mNumVertices );
	
	std::string msg = std::string();

	for ( glmd::uint32 t = 0; t < mesh->mNumFaces; ++t )
	{
		const aiFace* face = &mesh->mFaces[t];

		switch ( face->mNumIndices )
		{
			case 1: 
				msg = std::string("__FILE__(__LINE__): Unable to load model...Unsupported number of indices per face (1).");
				LOG_ERROR( msg );
				throw exception::Exception(msg);
	
			case 2: 
				msg = std::string("Unable to load model...Unsupported number of indices per face (2).");
				LOG_ERROR( msg );
				throw exception::Exception(msg);
	
			case 3:
				break;
	
			case 4:
				msg = std::string("Unable to load model...Unsupported number of indices per face (4).");
				LOG_ERROR( msg );
				throw exception::Exception(msg);
			
			default:
				msg = std::string("Unable to load model...Unknown number of indices per face.");
				LOG_ERROR( msg );
				throw exception::Exception(msg);
		}

		glm::detail::uint32 numIndices = face->mNumIndices;

		data.vertices.resize(currentIndex + numIndices);
		data.normals.resize(currentIndex + numIndices);
		data.textureCoordinates.resize(currentIndex + numIndices);
		data.colors.resize(currentIndex + numIndices);
		
		

		// go through all vertices in face
		for ( glmd::uint32 i = 0; i < numIndices; i++ )
		{
			// get group index for current index i
			int vertexIndex = face->mIndices[i];

			vertexIndexMap[ vertexIndex ] = currentIndex + i;

			if ( mesh->mNormals != 0 )
			{
				data.vertices[currentIndex + i] = glm::vec3(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);
				data.normals[currentIndex + i] = glm::vec3(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
			}

			if ( mesh->HasTextureCoords(0))
			{
				data.textureCoordinates[currentIndex + i] = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);
			}

			if ( mesh->mColors[0] != 0 )
			{
				data.colors[currentIndex + i] = glm::vec4(
					(float)mesh->mColors[0][vertexIndex].a,
					(float)mesh->mColors[0][vertexIndex].b,
					(float)mesh->mColors[0][vertexIndex].g,
					(float)mesh->mColors[0][vertexIndex].r
					);
			}
		}

		currentIndex += 3;
	}
	
	
	
	int temp = 0;
	
	// If we have any bones, load them
	if (mesh->mNumBones > 0)
	{
		data.bones.resize( data.vertices.size() );
		
		// Load bone data
		for (glmd::uint32 i = 0; i < mesh->mNumBones; i++)
		{
			glmd::uint32 boneIndex = boneIndexMap[ mesh->mBones[i]->mName.C_Str() ];
			
			for (glmd::uint32 j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				glmd::uint32 vertexID = vertexIndexMap[ mesh->mBones[i]->mWeights[j].mVertexId ];
				glmd::float32 weight = mesh->mBones[i]->mWeights[j].mWeight;

				temp++;
				data.bones[ vertexID ].addBoneWeight( boneIndex, weight );
			}
		}
	}
	
	std::cout << "Load results: " << mesh->mNumVertices << " " << mesh->mNumBones << " " << temp << " " << data.bones.size() << std::endl;

	LOG_DEBUG( "done loading mesh '" << filename << "'." );
	
	return data;
}

TextureData ModelLoader::loadTexture(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMaterial* material)
{
	assert( material != nullptr );
	
	TextureData data = TextureData();

	LOG_DEBUG( "Loading texture." );

	aiReturn texFound = AI_SUCCESS;
	aiString texPath;
	
	if ( material->GetTextureCount(aiTextureType_DIFFUSE) > 0 )
	{
		texFound = material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		
		// Error check
		if (texFound != AI_SUCCESS)
		{
			LOG_WARN( "Texture not found for model filename: " << filename );
			return data;
		}

		LOG_DEBUG( "Texture has filename: " << texPath.data );
		
		data.filename = texPath.data;
		
	}
	else
	{
		LOG_DEBUG( "No material specified." );
	}
	
	return data;
}

MaterialData ModelLoader::loadMaterial(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMaterial* material)
{	
	assert( material != nullptr );
		
	MaterialData data = MaterialData();

	LOG_DEBUG( "loading material." );
	aiColor4D c;
	
	// Set the material name
	data.name = name + "_material_" + std::to_string(index);

	LOG_DEBUG( "material name: " << data.name );

	data.diffuse[0] = 0.8f;
	data.diffuse[1] = 0.8f;
	data.diffuse[2] = 0.8f;
	data.diffuse[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &c) )
		utilities::color4ToVec4(&c, data.diffuse);

	//utilities::setFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	data.specular[0] = 0.0f;
	data.specular[1] = 0.0f;
	data.specular[2] = 0.0f;
	data.specular[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &c) )
		utilities::color4ToVec4(&c, data.specular);

	//utilities::setFloat4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	data.ambient[0] = 0.2f;
	data.ambient[1] = 0.2f;
	data.ambient[2] = 0.2f;
	data.ambient[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &c) )
		utilities::color4ToVec4(&c, data.ambient);

	//utilities::setFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	data.emission[0] = 0.0f;
	data.emission[1] = 0.0f;
	data.emission[2] = 0.0f;
	data.emission[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &c) )
		utilities::color4ToVec4(&c, data.emission);
	
	LOG_DEBUG( "done loading material." );
	
	return data;
}

glw::BoneData ModelLoader::loadBones(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMesh* mesh)
{
	assert( mesh != nullptr );
	
	glw::BoneData boneData = glw::BoneData();
	
	LOG_DEBUG( "loading boneData." );
	
	for (glmd::uint32 i = 0; i < mesh->mNumBones; i++) {
		glw::Bone data = glw::Bone();
		glmd::uint32 boneIndex = 0;
		
		// Set the bone name
		if (mesh->mBones[i]->mName.length > 0)
			data.name = std::string( mesh->mBones[i]->mName.C_Str() );
		else
			data.name = std::string( name ) + "_bone_" + std::to_string(index);
		
		LOG_DEBUG( "bone name: " << data.name );
		
		if (boneData.boneIndexMap.find(data.name) == boneData.boneIndexMap.end()) {
			boneIndex = boneData.boneIndexMap.size();
			boneData.boneTransform.push_back(data);
		}
		else {
			boneIndex = boneData.boneIndexMap[data.name];
		}

		boneData.boneIndexMap[data.name] = boneIndex;
		boneData.boneTransform[boneIndex].boneOffset = convertAssImpMatrix( &(mesh->mBones[i]->mOffsetMatrix) );
	} 

	LOG_DEBUG( "done loading boneData." );
	
	return boneData;
}

AnimationSet ModelLoader::loadAnimations(const std::string& name, const std::string& filename, const aiScene* scene)
{
	assert( scene != nullptr );
	
	AnimationSet animationSet = AnimationSet();
	
	std::stringstream msg;
	msg << "loading " << scene->mNumAnimations << " animation(s).";
	LOG_DEBUG( msg.str() );
	
	glm::mat4 globalInverseTransformation = convertAssImpMatrix( &(scene->mRootNode->mTransformation) );
	globalInverseTransformation = glm::inverse( globalInverseTransformation );
	
	animationSet.globalInverseTransformation = globalInverseTransformation;
	
	// Load BoneNodes
	const aiNode* assImpRootNode = scene->mRootNode;
	animationSet.rootBoneNode = loadBoneNode( assImpRootNode );
	
	for (glmd::uint32 i = 0; i < scene->mNumAnimations; i++)
	{
		// Load some basic animation data
		AnimationData animation = AnimationData();
		animation.name = std::string( scene->mAnimations[i]->mName.C_Str() );
		
		// Error check - animations with no name are not allowed
		if (animation.name.compare( std::string("") ) == 0)
		{
			LOG_WARN( "Animations with no name are not allowed." );
			
			animation.name = name + "_animation_" + std::to_string(i);
			LOG_WARN( "Setting animation name to: " << animation.name );
			// TODO: should we throw an exception?
			//throw exception::Exception(msg);
			
			// TODO: Should we skip to the next animation?
			//continue;
		}
		std::string msg = std::string("Loading animation...");
		
		animation.duration = scene->mAnimations[i]->mDuration;
		animation.ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
		
		// Load AnimatedBoneNodes for this animation
		for (glmd::uint32 j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			const aiNodeAnim* pNodeAnim = scene->mAnimations[i]->mChannels[j];
			
			glw::AnimatedBoneNode abn = glw::AnimatedBoneNode();
			abn.name = std::string( pNodeAnim->mNodeName.C_Str() );

			for (glmd::uint32 k = 0; k < pNodeAnim->mNumPositionKeys; k++)
			{
				abn.positionTimes.push_back( pNodeAnim->mPositionKeys[k].mTime );
				abn.positions.push_back( glm::vec3( pNodeAnim->mPositionKeys[k].mValue.x, pNodeAnim->mPositionKeys[k].mValue.y, pNodeAnim->mPositionKeys[k].mValue.z ) );
			}
			
			for (glmd::uint32 k = 0; k < pNodeAnim->mNumRotationKeys; k++)
			{
				const auto& rk = pNodeAnim->mRotationKeys[k];

				abn.rotationTimes.push_back( rk.mTime );

				glm::quat rotation = glm::quat( rk.mValue.w, rk.mValue.x, rk.mValue.y, rk.mValue.z );
				rotation = glm::normalize( rotation );
				abn.rotations.push_back( rotation );
			}
			
			for (glmd::uint32 k = 0; k < pNodeAnim->mNumScalingKeys; k++)
			{
				abn.scalingTimes.push_back( pNodeAnim->mScalingKeys[k].mTime );
				abn.scalings.push_back( glm::vec3( pNodeAnim->mScalingKeys[k].mValue.x, pNodeAnim->mScalingKeys[k].mValue.y, pNodeAnim->mScalingKeys[k].mValue.z ) );
			}
			
			
			// Add AnimatedBoneNode to the animation
			if( animation.animatedBoneNodes.find( abn.name ) == animation.animatedBoneNodes.end() )
			{
				animation.animatedBoneNodes[ abn.name ] = abn;
			}
			else 
			{
				// Warning - animated bone node already exists!
				LOG_WARN( "Animated bone node with name '" << abn.name << "' already exists!" );
			}
		}
		
		// Add animation to animation data
		if( animationSet.animations.find( animation.name ) == animationSet.animations.end() )
		{
			animationSet.animations[ animation.name ] = animation;
		}
		else 
		{
			// Warning - animated bone node already exists!
			LOG_WARN( "Animation with name '" << animation.name << "' already exists!" );
		}
	}

	LOG_DEBUG( "done loading animation." );
	
	return animationSet;
}

glw::BoneNode ModelLoader::loadBoneNode( const aiNode* node )
{
	glw::BoneNode boneNode = glw::BoneNode();
	boneNode.name = std::string( node->mName.C_Str() );
	boneNode.transformation = convertAssImpMatrix( &(node->mTransformation) );
	
	for (glmd::uint32 i = 0; i < node->mNumChildren; i++)
	{
		boneNode.children.push_back( loadBoneNode( node->mChildren[i] ) );
	}
	
	return boneNode;
}

glm::mat4 ModelLoader::convertAssImpMatrix(const aiMatrix4x4* m)
{
	return glm::mat4(
		m->a1, m->b1, m->c1, m->d1,
		m->a2, m->b2, m->c2, m->d2,
		m->a3, m->b3, m->c3, m->d3,
		m->a4, m->b4, m->c4, m->d4
	);
}

}
}
