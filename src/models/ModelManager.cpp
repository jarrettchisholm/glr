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

IModel* ModelManager::getModel(const std::string& name)
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


void ModelManager::loadModel(const std::string& name, const std::string& filename)
{
	std::cout << name << std::endl;
	std::cout << filename << std::endl;
	LOG_DEBUG( "Loading model '" + filename + "'." );

	if ( models_[name] != 0 )
	{
		LOG_DEBUG( "Model found...No need to load." );
		//return models_[filename].get();
	}

	// Note: We allow the modelData shared pointer to die at the end of this method
	std::vector< std::shared_ptr<ModelData> > modelData = modelLoader_.loadModel( name, filename );

	models_[name] = std::unique_ptr<Model>(new Model(modelData, openGlDevice_));

	LOG_DEBUG( "Done loading model '" + filename + "'." );

	//return models_[filename].get();
}

std::unique_ptr<IModel> ModelManager::createInstance(const std::string& name)
{
	LOG_DEBUG( "Creating model '" + name + "'." );
	
	auto it = models_.find(name);
	
	if ( it != models_.end() )
	{
		LOG_DEBUG( "Model found." );
		
		// Create a COPY of the model, and returns it wrapped in a unique pointer
		return std::unique_ptr<IModel>( new Model(*it->second.get()) );
	}

	LOG_DEBUG( "Model not found." );
	
	return std::unique_ptr<IModel>( nullptr );
}

}
}
