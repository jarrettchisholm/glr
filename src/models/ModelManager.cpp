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


namespace glr {
namespace models {
ModelManager::ModelManager(glw::IOpenGlDevice* openGlDevice)
	: openGlDevice_(openGlDevice)
{
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);
	
	materialManager_ = std::unique_ptr<glw::IMaterialManager>( new glw::MaterialManager(openGlDevice_) );
	textureManager_ = std::unique_ptr<glw::ITextureManager>( new glw::TextureManager(openGlDevice_) );
	meshManager_ = std::unique_ptr<glw::IMeshManager>( new glw::MeshManager(openGlDevice_) );
}

ModelManager::~ModelManager()
{
	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
}

/*
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
 */

/**
 *
 * @returns The raw pointer to a Model object.  The caller does not own the pointer - it is managed by
 * the ModelManager.
 */
IModel* ModelManager::loadModel(const std::string filename)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading model '" << filename << "'.";

	if ( models_[filename] != 0 )
	{
		BOOST_LOG_TRIVIAL(debug) << "Model found.";
		return models_[filename].get();
	}



	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	const aiScene* scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if ( scene == 0 )
	{
		BOOST_LOG_TRIVIAL(debug) << "Unable to load model...";
		return 0;
	}


	//get_bounding_box(&scene_min,&scene_max);
	//scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
	//scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
	//scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

	//std::shared_ptr<aiScene> sharedScene = std::shared_ptr<aiScene>(scene);
	models_[filename] = std::unique_ptr<Model>(new Model(scene, openGlDevice_, meshManager_.get(), materialManager_.get(), textureManager_.get()));

	// cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	// TODO: Should I use raw pointer instead of wrapping it in shared_ptr???
	aiReleaseImport(scene);

	BOOST_LOG_TRIVIAL(debug) << "Done loading model '" << filename << "'.";

	return models_[filename].get();
}

/*
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
 */
}
}
