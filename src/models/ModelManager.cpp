#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "models/ModelManager.hpp"

#include "common/utilities/ImageLoader.hpp"

namespace glr
{
namespace models
{

ModelManager::ModelManager(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	modelLoader_ = ModelLoader();
	
	models_ = std::map< std::string, std::unique_ptr<IModel> >();
	modelInstances_ = std::vector< std::unique_ptr<IModel> >();
	
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
	auto model = modelLoader_.loadModel( name, filename );

	models_[name] = std::move(model);

	LOG_DEBUG( "Done loading model '" + filename + "'." );
}

IModel* ModelManager::getModelTemplate(glm::detail::uint32 id)
{
}

IModel* ModelManager::getModelTemplate(const std::string& name)
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

void ModelManager::destroyModelTemplate(glm::detail::uint32 id)
{
}

void ModelManager::destroyModelTemplate(const std::string& name)
{
}

void ModelManager::destroyModelTemplate(IModel* model)
{
}

IModel* ModelManager::createInstance(const std::string& name)
{
	LOG_DEBUG( "Creating model '" + name + "'." );
	
	auto it = models_.find(name);
	
	if ( it != models_.end() )
	{
		LOG_DEBUG( "Model found." );
		
		// Create a COPY of the model and returns a pointer
		modelInstances_.push_back(std::unique_ptr<IModel>( new Model(*it->second.get()) ));
		
		return modelInstances_.back().get();
	}

	LOG_DEBUG( "Model not found." );
	
	return nullptr;
}

void ModelManager::destroyInstance(glm::detail::uint32 id)
{
}

void ModelManager::destroyInstance(IModel* model)
{
}

IModel* ModelManager::getInstance(glm::detail::uint32 id)
{
}

}
}
