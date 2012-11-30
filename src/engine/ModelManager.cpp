/*
 * ModelManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "ModelManager.h"

#include "../common/utilities/ImageLoader.h"


namespace icee {

namespace engine {

ModelManager::ModelManager() {
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);
	
}

ModelManager::~ModelManager() {
	// cleanup - calling 'aiReleaseImport' is important, as the library 
	// keeps internal resources until the scene is freed again. Not 
	// doing so can cause severe resource leaking.
	aiReleaseImport(scene);

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
}

void ModelManager::testLoadTexture() {
	glBindTexture(GL_TEXTURE_2D, textureid_);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // i don't combine the color with the original surface color, use only the texture map.
    
    utilities::ImageLoader il;
    utilities::Image* image = il.loadImageData("/home/jarrett/projects/chisholmsoft/models/Crea2.jpg");
    BOOST_LOG_TRIVIAL(debug) << "ModelManager::testLoadTexture: image: " << image->width << "x" << image->height;
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

Model* ModelManager::loadModel(const char* path) {
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	std::map<std::string, GLuint*> textureIdMap_;
	GLuint*	textureIds_;

	recursiveLoadTexture(scene, scene->mRootNode, textureIdMap_, textureIds_);

	if (scene) {
		//get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		
		Model* model = new Model(scene);
		
	}
	
	return 0;
}

void ModelManager::recursiveTextureLoad(const struct aiScene *sc, const struct aiNode* nd) {
	if (scene->HasTextures()) {
		BOOST_LOG_TRIVIAL(debug) << "Support for meshes with embedded textures is not implemented";
		exit 1;
	}

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m = 0; m < scene->mNumMaterials; m++) {
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS) {
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			if ( textureIdMap[path.data] == 0 ) {
				//fill map with textures, pointers still NULL yet
				textureIdMap[path.data] = 0;
				texIndex++;
			}
		}
	}
	
	int numTextures = textureIdMap.size();
	
	/* create and fill array with GL texture ids */
	textureIds_ = new GLuint[numTextures];
	/* Texture name generation */
	glGenTextures(numTextures, textureIds_);
	
	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
	
	std::string basepath = "/home/jarrett/projects/chisholmsoft/models/";
	
	for (int i=0; i<numTextures; i++) {
		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		(*itr).second =  &textureIds_[i];	  // save texture id for filename in map
		itr++;								  // next texture

		utilities::ImageLoader il;
	    utilities::Image* image = il.loadImageData(basepath + filename);
	    BOOST_LOG_TRIVIAL(debug) << "ModelManager::recursiveTextureLoad: image: " << image->width << "x" << image->height;

		glBindTexture(GL_TEXTURE_2D, textureIds_[i]);
		// If the u,v coordinates overflow the range 0,1 the image is repeated
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	    
	    // i don't combine the color with the original surface color, use only the texture map.
	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    
	    // And create 2d mipmaps for the minifying function
	    //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
	    
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 
		delete image;
	 
		// error check
		GLenum huboError = glGetError();
		if(huboError){
			BOOST_LOG_TRIVIAL(debug) << "ModelManager::recursiveTextureLoad:: error loading texture in opengl";
			exit 1;
		}
	}
	
}

void ModelManager::testDrawTest1() {	
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



}

}
