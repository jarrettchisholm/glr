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

ModelManager::ModelManager(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	modelLoader_ = ModelLoader();
	
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);
}

ModelManager::~ModelManager()
{
	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
}

/**
 *
 * @return The raw pointer to a Model object.  The caller does not own the pointer - it is managed by
 * the ModelManager.
 */
IModel* ModelManager::getModel(const std::string name)
{
	LOG_DEBUG( "Retrieving model '" + name + "'." );
	
	if ( models_.find(name) != models_.end() )
	{
		LOG_DEBUG( "Model found." );
		return models_[name].get();
	}

	LOG_DEBUG( "Model not found." );
	
	return nullptr;
}

/**
 * Loads the model from the given filename.
 */
void ModelManager::loadModel(const std::string name, const std::string filename)
{
	LOG_DEBUG( "Loading model '" + filename + "'." );

	if ( models_[name] != 0 )
	{
		LOG_DEBUG( "Model found...No need to load." );
		//return models_[filename].get();
	}

	// Note: We allow the modelData shared pointer to die at the end of this method
	std::vector< std::shared_ptr<ModelData> > modelData = modelLoader_.loadModel( filename );

	models_[name] = std::unique_ptr<Model>(new Model(modelData, openGlDevice_));

	LOG_DEBUG( "Done loading model '" + filename + "'." );

	//return models_[filename].get();
}

/**
 * @return A unique pointer to a model object.  The model is a copy of the model that was loaded
 * from the specified file.
 */
std::unique_ptr<IModel> ModelManager::createModel(const std::string name)
{
	LOG_DEBUG( "Creating model '" + name + "'." );
	
	if ( models_.find(name) != models_.end() )
	{
		LOG_DEBUG( "Model found." );
		
		// Create a COPY of the model, and returns it wrapped in a unique pointer
		return std::unique_ptr<IModel>( new Model(*models_[name].get()) );
	}

	LOG_DEBUG( "Model not found." );
	
	return std::unique_ptr<IModel>( nullptr );
}

// TESTING
std::unique_ptr<IModel> ModelManager::createModel(glw::Mesh* mesh)
{
	LOG_DEBUG( "NOTE: Method 'ModelManager::createModel(glw::IMesh* mesh)' is for testing purposes only!" );
	
	return std::unique_ptr<IModel>( new Model(mesh, openGlDevice_) );
}

}
}
