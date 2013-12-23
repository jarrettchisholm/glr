/*
 * ModelLoader.cpp
 *
 *  Created on: 2011-05-08
 *	  Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ModelLoader.h"

#include "../common/utilities/ImageLoader.h"
#include "../common/utilities/AssImpUtilities.h"
#include "../exceptions/GlException.h"


namespace glr {
namespace models {
	
ModelLoader::ModelLoader()
{
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);
}

ModelLoader::~ModelLoader()
{
	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
}

/**
 * Loads model data from the file specified by filename.
 * 
 * @param filename The file we want to load
 * 
 * @returns A vector of ModelData objects (as shared pointers).  We do this so as not to have to copy the
 * model data when we return from this method.
 */
std::vector< std::shared_ptr<ModelData> > ModelLoader::loadModel(const std::string& filename)
{
	return loadModel(filename, filename);
}

std::vector< std::shared_ptr<ModelData> > ModelLoader::loadModel(const std::string& name, const std::string& filename)
{
	LOG_DEBUG( "Loading model '" + name + "' - " + filename + "." );

	std::vector< std::shared_ptr<ModelData> > modelData = std::vector< std::shared_ptr<ModelData> >();	

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
		std::string msg = std::string("Support for meshes with embedded textures is not implemented.");
		LOG_ERROR( msg );
		throw exception::Exception(msg);
	}
	
	modelData.resize( scene->mNumMeshes );
	
	glm::mat4 globalInverseTransformation = convertAssImpMatrix( &(scene->mRootNode->mTransformation) );
	globalInverseTransformation = glm::inverse( globalInverseTransformation );
	
	AnimationSet animationSet = loadAnimations(name, filename, scene);
	
	std::stringstream msg;
	msg << "Model has " << modelData.size() << " meshes.";
	LOG_DEBUG( msg.str() );
	
	int NumVertices = 0;
	for ( glmd::uint32 i=0; i < modelData.size(); i++ )
	{
		modelData[i] = std::shared_ptr<ModelData>(new ModelData());
		
		modelData[i]->boneData = loadBones(name, filename, i, scene->mMeshes[i]);
		modelData[i]->meshData = loadMesh( name, filename, i, scene->mMeshes[i], modelData[i]->boneData.boneIndexMap );
		modelData[i]->textureData = loadTexture( name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ] );
		modelData[i]->materialData = loadMaterial( name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ] );
		modelData[i]->animationSet = animationSet;
		modelData[i]->globalInverseTransformation = globalInverseTransformation;
		
		std::cout << "NumVertices: " << NumVertices << std::endl;
		NumVertices += scene->mMeshes[i]->mNumVertices;
	}
	

	// cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	// TODO: Should I use raw pointer instead of wrapping it in shared_ptr???
	aiReleaseImport(scene);

	LOG_DEBUG( "Done loading model '" + filename + "'." );

	return modelData;
}


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
MeshData ModelLoader::loadMesh(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMesh* mesh, std::map< std::string, glmd::uint32 >& boneIndexMap)
{
	MeshData data = MeshData();
	
	LOG_DEBUG( "loading mesh '" + filename + "'." );
	
	// Set the mesh name
	if (mesh->mName.length > 0)
		data.name = std::string( mesh->mName.C_Str() );
	else
		data.name = name + "_mesh_" + std::to_string(index);
	LOG_DEBUG( "mesh name: " + data.name );

	// Load vertices, normals, texture coordinates, and colors
	glm::detail::uint32 currentIndex = 0;
	
	std::vector< glmd::uint32 > vertexIndexMap = std::vector< glmd::uint32 >();
	vertexIndexMap.resize( mesh->mNumVertices );
	
	std::string msg = std::string();
	
	/*
	data.vertices.resize(mesh->mNumVertices);
	data.normals.resize(mesh->mNumVertices);
	data.textureCoordinates.resize(mesh->mNumVertices);
	data.colors.resize(mesh->mNumVertices);
	
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiColor4D Zero4D(0.0f, 0.0f, 0.0f, 0.0f);
	
	for ( glmd::uint32 i = 0; i < mesh->mNumVertices; i++ )
	{
		const aiVector3D* pPos      = &(mesh->mVertices[i]);
        const aiVector3D* pNormal   = &(mesh->mNormals[i]);
        const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
        const aiColor4D*  pColor    = mesh->mColors[0] != 0 ? &(mesh->mColors[0][i]) : &Zero4D;
        
        data.vertices.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        data.normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        data.textureCoordinates.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));  
		data.colors.push_back( glm::vec4(
				pColor->a,
				pColor->b,
				pColor->g,
				pColor->r
			)
		);
	}
	*/
	
	
	for ( glmd::uint32 t = 0; t < mesh->mNumFaces; ++t )
	{
		const aiFace* face = &mesh->mFaces[t];
		GLenum face_mode;

		switch ( face->mNumIndices )
		{
			case 1: 
				msg = std::string("Unable to load model...Unsupported number of indices per face (1).");
				LOG_ERROR( msg );
				throw exception::Exception(msg);
	
			case 2: 
				msg = std::string("Unable to load model...Unsupported number of indices per face (2).");
				LOG_ERROR( msg );
				throw exception::Exception(msg);
	
			case 3: 
				face_mode = GL_TRIANGLES; 
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

			//utilities::AssImpUtilities::color4_to_vec4(&mesh->mColors[0][vertexIndex], data.colors[data.colors.size() + i]);
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
				//std::cout << vertexID << " " << weight << std::endl;
				
				temp++;
				data.bones[ vertexID ].addBoneWeight( boneIndex, weight );
			}
		}
	}
	
	std::cout << "Load results: " << mesh->mNumVertices << " " << mesh->mNumBones << " " << temp << " " << data.bones.size() << std::endl;
	// Fill in any empty weights
	
	// DEBUGGING
	/*
	std::ofstream file;
	file.open("bones.txt");
	for (glmd::uint32 i = 0; i < data.bones.size(); i++)
	{
		//data.bones[ i ].normalize();
		file << i << ": " << data.bones[ i ].toString() << "\n";
	}
	file.close();
	
	file.open("vertices.txt");
	for (glmd::uint32 i = 0; i < data.vertices.size(); i++)
	{
		file << i << ": " << glm::to_string(data.vertices[ i ]) << "\n";
	}
	file.close();
	
	file.open("normals.txt");
	for (glmd::uint32 i = 0; i < data.normals.size(); i++)
	{
		file << i << ": " << glm::to_string(data.normals[ i ]) << "\n";
	}
	file.close();
	
	file.open("textureCoordinates.txt");
	for (glmd::uint32 i = 0; i < data.textureCoordinates.size(); i++)
	{
		file << i << ": " << glm::to_string(data.textureCoordinates[ i ]) << "\n";
	}
	file.close();
	*/
	LOG_DEBUG( "done loading mesh '" + filename + "'." );

	//materialMap_[n] = scene->mMeshes[n]->mMaterialIndex;
	//textureMap_[n] = scene->mMeshes[n]->mMaterialIndex;
	
	return data;
}

/**
 * Load the texture filename for the given material.
 * 
 * @param filename The file being loaded
 * @param index The current index of the texture being loaded
 * @param mesh The AssImp material data structure to load data from.  Must not be null.
 */
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
			LOG_WARN( "Texture not found for model filename: " + filename );
			return data;
		}
		
		std::stringstream msg;
		msg << "Texture has filename: " << texPath.data;
		LOG_DEBUG( msg.str() );
		
		data.filename = texPath.data;
		
	}
	else
	{
		LOG_DEBUG( "No material specified." );
	}
	return data;
}

/**
 * Load the material data for the given material.
 * 
 * @param filename The file being loaded
 * @param index The current index of the material being loaded
 * @param material The AssImp material data structure to load data from.  Must not be null.
 */
MaterialData ModelLoader::loadMaterial(const std::string& name, const std::string& filename, glmd::uint32 index, const aiMaterial* material)
{	
	assert( material != nullptr );
		
	MaterialData data = MaterialData();

	LOG_DEBUG( "loading material." );
	aiColor4D c;
	
	// Set the material name
	data.name = name + "_material_" + std::to_string(index);

	LOG_DEBUG( "material name: " + data.name );

	data.diffuse[0] = 0.8f;
	data.diffuse[1] = 0.8f;
	data.diffuse[2] = 0.8f;
	data.diffuse[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, data.diffuse);

	//utilities::AssImpUtilities::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	data.specular[0] = 0.0f;
	data.specular[1] = 0.0f;
	data.specular[2] = 0.0f;
	data.specular[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, data.specular);

	//utilities::AssImpUtilities::set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	data.ambient[0] = 0.2f;
	data.ambient[1] = 0.2f;
	data.ambient[2] = 0.2f;
	data.ambient[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, data.ambient);

	//utilities::AssImpUtilities::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	data.emission[0] = 0.0f;
	data.emission[1] = 0.0f;
	data.emission[2] = 0.0f;
	data.emission[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &c))
		utilities::AssImpUtilities::color4_to_vec4(&c, data.emission);
	
	LOG_DEBUG( "done loading material." );
	
	return data;
}

/**
 * Will load the bone data for the given AssImp mesh.
 * 
 * @param filename The file being loaded
 * @param index The current index of the mesh being loaded
 * @param mesh The AssImp mesh data structure to load data from.  Must not be null.
 */
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
		
		LOG_DEBUG( "bone name: " + data.name );
		
		if (boneData.boneIndexMap.find(data.name) == boneData.boneIndexMap.end()) {
			boneIndex = boneData.boneIndexMap.size();
			boneData.boneTransform.push_back(data);
		}
		else {
			boneIndex = boneData.boneIndexMap[data.name];
		}

		boneData.boneIndexMap[data.name] = boneIndex;
		boneData.boneTransform[boneIndex].boneOffset = convertAssImpMatrix( &(mesh->mBones[i]->mOffsetMatrix) );

		//for (glmd::uint32 j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) {
		//	glmd::uint32 VertexID = m_Entries[MeshIndex].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
		//	float Weight = mesh->mBones[i]->mWeights[j].mWeight; 
		//	Bones[VertexID].AddBoneData(boneIndex, Weight);
		//}
	} 

	LOG_DEBUG( "done loading boneData." );
	
	return boneData;
}

/**
 * Will load the animations for the given scene.
 * 
 * @param filename The file being loaded
 * @param scene The AssImp scene data structure to load animation data from.  Must not be null.
 */
AnimationSet ModelLoader::loadAnimations(const std::string& name, const std::string& filename, const aiScene* scene)
{
	assert( scene != nullptr );
	
	AnimationSet animationSet = AnimationSet();
	
	std::stringstream msg;
	msg << "loading " << scene->mNumAnimations << " animation(s).";
	LOG_DEBUG( msg.str() );
	
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
			LOG_WARN( "Setting animation name to: " + animation.name );
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
			
			AnimatedBoneNode abn = AnimatedBoneNode();
			abn.name = std::string( pNodeAnim->mNodeName.C_Str() );
			
			//std::cout << "loading abn: " << abn.name << " " << pNodeAnim->mNumPositionKeys << " " << pNodeAnim->mNumRotationKeys << " " << pNodeAnim->mNumScalingKeys <<std::endl;
			
			for (glmd::uint32 k = 0; k < pNodeAnim->mNumPositionKeys; k++)
			{
				abn.positionTimes.push_back( pNodeAnim->mPositionKeys[k].mTime );
				abn.positions.push_back( glm::vec3( pNodeAnim->mPositionKeys[k].mValue.x, pNodeAnim->mPositionKeys[k].mValue.y, pNodeAnim->mPositionKeys[k].mValue.z ) );
			}
			
			for (glmd::uint32 k = 0; k < pNodeAnim->mNumRotationKeys; k++)
			{
				abn.rotationTimes.push_back( pNodeAnim->mRotationKeys[k].mTime );
				abn.rotations.push_back( 
					glm::quat( 
						pNodeAnim->mRotationKeys[k].mValue.w,
						pNodeAnim->mRotationKeys[k].mValue.x,
						pNodeAnim->mRotationKeys[k].mValue.y,
						pNodeAnim->mRotationKeys[k].mValue.z
					)
				);
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
				LOG_WARN( "Animated bone node with name '" + abn.name + "' already exists!" );
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
			LOG_WARN( "Animation with name '" + animation.name + "' already exists!" );
		}
	}

	LOG_DEBUG( "done loading animation." );
	
	return animationSet;
}

/**
 * Helper method - recursively loads bone node information from the AssImp node.
 * 
 * @param node An AssImp Node.
 * 
 * @return The BoneNode containing the bone information from the given AssImp node.
 */
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

/**
 * Helper method - converts an AssImp 4x4 matrix into a glm 4x4 matrix.
 * 
 * @param m A 4x4 AssImp matrix
 * 
 * @return A 4x4 glm matrix
 */
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
