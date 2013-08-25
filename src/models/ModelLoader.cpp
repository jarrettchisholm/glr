/*
 * ModelLoader.cpp
 *
 *  Created on: 2011-05-08
 *	  Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

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

/*
   void ModelLoader::testLoadTexture() {
		glBindTexture(GL_TEXTURE_2D, textureid_);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // i don't combine the color with the original surface color, use only the texture map.

	utilities::ImageLoader il;
	utilities::Image* image = il.loadImageData("/home/jarrett/projects/chisholmsoft/models/Crea2.jpg");
	BOOST_LOG_TRIVIAL(debug) << "ModelLoader::testLoadTexture: image: " << image->width << "x" << image->height;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

	// And create 2d mipmaps for the minifying function
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		GLenum huboError = glGetError();
		if(huboError){
				BOOST_LOG_TRIVIAL(debug) << "HtmlGuiComponent::initialize: error loading texture in opengl";
		}

	delete image;
   }
 */

/**
 *
 * @returns A vector of ModelData objects (as shared pointers).  We do this so as not to have to copy the
 * model data when we return from this method.
 */
std::vector< std::shared_ptr<ModelData> > ModelLoader::loadModel(const std::string path)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading model '" << path << "'.";

	std::vector< std::shared_ptr<ModelData> > modelData = std::vector< std::shared_ptr<ModelData> >();	

	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	// Error checking
	if ( scene == nullptr )
	{		
		std::string msg = std::string("Unable to load model...");

		BOOST_LOG_TRIVIAL(error) << msg;
		
		throw exception::Exception(msg);
	}
	else if ( scene->HasTextures() )
	{		
		std::string msg = std::string("Support for meshes with embedded textures is not implemented.");

		BOOST_LOG_TRIVIAL(error) << msg;
		
		throw exception::Exception(msg);
	}
	
	modelData.resize( scene->mNumMeshes );
	
	AnimationData animationData = loadAnimation(path, scene);
	
	for ( glmd::uint32 i=0; i < modelData.size(); i++ )
	{
		modelData[i] = std::shared_ptr<ModelData>(new ModelData());
		
		modelData[i]->boneData = loadBones(path, i, scene->mMeshes[i]);
		modelData[i]->meshData = loadMesh( path, i, scene->mMeshes[i], modelData[i]->boneData.boneIndexMap );
		modelData[i]->textureData = loadTexture( path, i, scene->mMaterials[i] );
		modelData[i]->materialData = loadMaterial( path, i, scene->mMaterials[i] );
		modelData[i]->animationData = animationData;
	}
	

	// cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	// TODO: Should I use raw pointer instead of wrapping it in shared_ptr???
	aiReleaseImport(scene);

	BOOST_LOG_TRIVIAL(debug) << "Done loading model '" << path << "'.";

	return modelData;
}

/*
   void ModelLoader::testDrawTest1() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_TEXTURE_2D );

		glBindTexture(GL_TEXTURE_2D, textureid_);
		glBegin(GL_QUADS);
			glTexCoord2f(0.f, 0.f); glVertex3f(-10.f, -10.f, 0.f);
			glTexCoord2f(0.f, 1.f); glVertex3f(-10.f,  10.f, 0.f);
			glTexCoord2f(1.f, 1.f); glVertex3f( 10.f,  10.f, 0.f);
			glTexCoord2f(1.f, 0.f); glVertex3f( 10.f, -10.f, 0.f);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
   }
 */

/**
 * Load the vertex, normal, texture, and color data for the given mesh.
 * 
 * Note: We only support models that have faces with 3 points (i.e. triangles)
 * 
 * @param path The file being loaded
 * @param index The current index of the mesh being loaded
 * @param mesh The AssImp mesh data structure to load data from.  Must not be null.
 * @param boneIndexMap A map associating an AssImp bone name with a bone index (of type uint32).  Note that this variable is NOT const - this is
 * only so that we can access elements in the map using the operator[] operator.
 */
MeshData ModelLoader::loadMesh(const std::string path, glmd::uint32 index, const aiMesh* mesh, std::map< std::string, glmd::uint32 >& boneIndexMap)
{
	MeshData data = MeshData();
	
	BOOST_LOG_TRIVIAL(debug) << "loading mesh...";
	
	// Set the mesh name
	if (mesh->mName.length > 0)
		data.name = std::string( mesh->mName.C_Str() );
	else
		data.name = std::string( path ) + "_mesh_" + std::to_string(index);


	// Load vertices, normals, texture coordinates, and colors
	glm::detail::uint32 currentIndex = 0;
	
	std::vector< glmd::uint32 > vertexIndexMap = std::vector< glmd::uint32 >();
	vertexIndexMap.resize( mesh->mNumFaces * 3 );
	
	std::string msg = std::string();
	
	for ( glmd::uint32 t = 0; t < mesh->mNumFaces; ++t )
	{
		const aiFace* face = &mesh->mFaces[t];
		GLenum face_mode;

		switch ( face->mNumIndices )
		{
			case 1: 
				//face_mode = GL_POINTS;
				msg = std::string("Unable to load model...Unsupported number of indices per face.");
				BOOST_LOG_TRIVIAL(error) << msg;
				throw exception::Exception(msg);
				break;
	
			case 2: 
				//face_mode = GL_LINES; 
				msg = std::string("Unable to load model...Unsupported number of indices per face.");
				BOOST_LOG_TRIVIAL(error) << msg;
				throw exception::Exception(msg);
				break;
	
			case 3: 
				face_mode = GL_TRIANGLES; 
				break;
	
			default:
				//face_mode = GL_POLYGON; 
				msg = std::string("Unable to load model...Unsupported number of indices per face.");
				BOOST_LOG_TRIVIAL(error) << msg;
				throw exception::Exception(msg);
				break;
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
	
	
	
	data.bones.resize( data.vertices.size() );
	
	// Load bone data
	for (glmd::uint32 i = 0; i < mesh->mNumBones; i++)
	{
		glmd::uint32 boneIndex = boneIndexMap[ mesh->mBones[i]->mName.C_Str() ];
		
		for (glmd::uint32 j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			glmd::uint32 vertexID = vertexIndexMap[ mesh->mBones[i]->mWeights[j].mVertexId ];
			glmd::float32 weight = mesh->mBones[i]->mWeights[j].mWeight; 
			
			data.bones[ vertexID ] = glm::vec2( boneIndex, weight );
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "done loading mesh...";

	//materialMap_[n] = scene->mMeshes[n]->mMaterialIndex;
	//textureMap_[n] = scene->mMeshes[n]->mMaterialIndex;
	
	return data;
}

/**
 * Load the texture filename for the given material.
 * 
 * @param path The file being loaded
 * @param index The current index of the texture being loaded
 * @param mesh The AssImp material data structure to load data from.  Must not be null.
 */
TextureData ModelLoader::loadTexture(const std::string path, glmd::uint32 index, const aiMaterial* material)
{
	TextureData data = TextureData();

	BOOST_LOG_TRIVIAL(debug) << "Loading texture.";

	aiReturn texFound = AI_SUCCESS;
	aiString texPath;

	texFound = material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
	
	// Error check
	if (texFound != AI_SUCCESS)
	{
		BOOST_LOG_TRIVIAL(warning) << "Texture not found for model path: " << path;
		return data;
	}
	
	BOOST_LOG_TRIVIAL(debug) << "Texture has path: " << texPath.data;

	data.filename = texPath.data;
	
	return data;
}

/**
 * Load the material data for the given material.
 * 
 * @param path The file being loaded
 * @param index The current index of the material being loaded
 * @param mesh The AssImp material data structure to load data from.  Must not be null.
 */
MaterialData ModelLoader::loadMaterial(const std::string path, glmd::uint32 index, const aiMaterial* material)
{		
	MaterialData data = MaterialData();

	BOOST_LOG_TRIVIAL(debug) << "loading material...";
	aiColor4D c;
	
	// Set the material name
	data.name = std::string( path ) + "_material_" + std::to_string(index);

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
	
	BOOST_LOG_TRIVIAL(debug) << "done loading material.";
	
	return data;
}

/**
 * Will load the bone data for the given AssImp mesh.
 * 
 * @param path
 * @param index
 * @param mesh
 */
BoneData ModelLoader::loadBones(const std::string path, glmd::uint32 index, const aiMesh* mesh)
{
	BoneData boneData = BoneData();
	
	BOOST_LOG_TRIVIAL(debug) << "loading boneData...";
	
	for (glmd::uint32 i = 0; i < mesh->mNumBones; i++) {
		Bone data = Bone();
		glmd::uint32 boneIndex = 0;
		
		// Set the bone name
		if (mesh->mBones[i]->mName.length > 0)
			data.name = std::string( mesh->mBones[i]->mName.C_Str() );
		else
			data.name = std::string( path ) + "_bone_" + std::to_string(index);

		if (boneData.boneIndexMap.find(data.name) == boneData.boneIndexMap.end()) {
			boneIndex = boneData.boneIndexMap.size();
			boneData.boneTransform.push_back(data);
		}
		else {
			boneIndex = boneData.boneIndexMap[data.name];
		}

		boneData.boneIndexMap[data.name] = boneIndex;
		boneData.boneTransform[boneIndex].boneOffset = convertAssImpMatrix( mesh->mBones[i]->mOffsetMatrix );

		//for (glmd::uint32 j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) {
		//	glmd::uint32 VertexID = m_Entries[MeshIndex].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
		//	float Weight = mesh->mBones[i]->mWeights[j].mWeight; 
		//	Bones[VertexID].AddBoneData(boneIndex, Weight);
		//}
	} 

	BOOST_LOG_TRIVIAL(debug) << "done loading boneData...";
	
	return boneData;
}

/**
 * Loads the animation(s) for the given scene.
 */
AnimationData ModelLoader::loadAnimation(const std::string path, const aiScene* scene)
{
	AnimationData animation = AnimationData();
	
	BOOST_LOG_TRIVIAL(debug) << "loading animations...";
	
	// TODO: load BoneNodes
	
	for (glmd::uint32 i = 0; i < scene->mNumAnimations; i++)
	{
		Animation animation = Animation();
		animation.name = std::string( scene->mAnimations[i]->mName.C_Str() );
		animation.duration = scene->mAnimations[i]->mDuration;
		animation.ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
		
		// TODO: load AnimatedBoneNodes for this animation
	} 

	BOOST_LOG_TRIVIAL(debug) << "done loading animation...";
	
	return animation;
}

/**
 * Helper method - converts an AssImp 4x4 matrix into a glm 4x4 matrix.
 * 
 * @param m A 4x4 AssImp matrix
 * 
 * @return A 4x4 glm matrix
 */
glm::mat4 ModelLoader::convertAssImpMatrix(aiMatrix4x4 m)
{
	return glm::mat4 (
		1.0f, 0.0f, 0.0f, m.a4,
		0.0f, 1.0f, 0.0f, m.b4,
		0.0f, 0.0f, 1.0f, m.c4,
		0.0f, 0.0f, 0.0f, m.d4
	);
}

}
}
