#include <utility>

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

ModelManager::ModelManager()
{
	openGlDevice_ = nullptr;
}

ModelManager::ModelManager(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	modelLoader_ = std::unique_ptr<ModelLoader>( new ModelLoader(openGlDevice_) );
	
	models_ = std::vector< std::unique_ptr<Model> >();
	modelInstances_ = std::vector< std::unique_ptr<IModel> >();
	
	idManager_ = IdManager();
	
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

IModel* ModelManager::getModelTemplate(Id id) const
{
	LOG_DEBUG( "Retrieving model template with id '" << id << "'." );

	return getModel(id);
}

IModel* ModelManager::getModelTemplate(const std::string& name) const
{
	LOG_DEBUG( "Retrieving model template with name '" << name << "'." );

	return getModel(name);
}

Model* ModelManager::getModel(Id id) const
{
	auto findFunction = [&id](const std::unique_ptr<Model>& node) { return node->getId() == id; };

	auto it = std::find_if(models_.begin(), models_.end(), findFunction);
	
	if (it != models_.end())
	{
		LOG_DEBUG( "Model template found." );
		return it->get();
	}

	LOG_DEBUG( "Model template not found." );
	
	return nullptr;
}

Model* ModelManager::getModel(const std::string& name) const
{
	auto findFunction = [&name](const std::unique_ptr<Model>& node) { return node->getName() == name; };

	auto it = std::find_if(models_.begin(), models_.end(), findFunction);
	
	if (it != models_.end())
	{
		LOG_DEBUG( "Model template found." );
		return it->get();
	}

	LOG_DEBUG( "Model template not found." );
	
	return nullptr;
}

void ModelManager::loadModel(const std::string& name, const std::string& filename, bool initialize)
{
	LOG_DEBUG( "Loading model '" + filename + "'." );

	auto model = getModelTemplate(name);

	if ( model != nullptr )
	{
		LOG_DEBUG( "Model found...No need to load." );
		return;
	}

	models_.push_back( modelLoader_->loadModel( name, filename, idManager_ ) );

	LOG_DEBUG( "Done loading model '" + filename + "'." );
}

void ModelManager::destroyModelTemplate(Id id)
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
	LOG_DEBUG( "Creating model instance from model template with name '" + name + "'." );
	
	auto model = getModel(name);
	
	if ( model != nullptr )
	{
		LOG_DEBUG( "Model template found." );
		
		std::string newName = name + std::string("_") + std::to_string(modelInstances_.size());
		// Create a COPY of the model template
		modelInstances_.push_back( std::unique_ptr<IModel>( new Model(idManager_.createId(), std::move(newName), *model) ) );
		
		return modelInstances_.back().get();
	}

	LOG_WARN( "Model template not found." );
	
	return nullptr;
}

void ModelManager::destroyInstance(Id id)
{
}

void ModelManager::destroyInstance(IModel* model)
{
}

IModel* ModelManager::getInstance(Id id) const
{
	LOG_DEBUG( "Retrieving model instance with id '" << id << "'." );

	auto findFunction = [&id](const std::unique_ptr<IModel>& node) { return node->getId() == id; };
	
	auto it = std::find_if(modelInstances_.begin(), modelInstances_.end(), findFunction);
	
	if (it != modelInstances_.end())
	{
		LOG_DEBUG( "Model instance found." );
		return it->get();
	}

	LOG_DEBUG( "Model instance not found." );
	
	return nullptr;
}

void ModelManager::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void ModelManager::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void ModelManager::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void ModelManager::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
}

template<class Archive> void ModelManager::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::split_member(ar, *this, version);
}

template<class Archive> void ModelManager::save(Archive& ar, const unsigned int version) const
{
	boost::serialization::void_cast_register<ModelManager, IModelManager>(
		static_cast<ModelManager*>(nullptr),
		static_cast<IModelManager*>(nullptr)
	);
	
	auto size = models_.size();
	ar & size;
	for (auto& it : models_)
	{
		ar & it->getName() & *(it.get());
	}
}

template<class Archive> void ModelManager::load(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<ModelManager, IModelManager>(
		static_cast<ModelManager*>(nullptr),
		static_cast<IModelManager*>(nullptr)
	);

	std::vector< std::unique_ptr<Model> >::size_type modelsSize = 0;
	ar & modelsSize;

	models_ = std::vector< std::unique_ptr<Model> >();

	for (glmd::uint32 i=0; i < modelsSize; i++)
	{
		// TODO: Create new id?  Or just save/load existing id?
		auto s = std::string();
		ar & s;
		
		Id id = idManager_.createId();
		
		auto model = std::unique_ptr<Model>( new Model(id, s, openGlDevice_) );
		ar & *(model.get());
		
		models_.push_back( std::move(model) );
	}
}

}
}

BOOST_CLASS_EXPORT(glr::models::IModelManager)
BOOST_CLASS_EXPORT_GUID(glr::models::ModelManager, "glr::models::ModelManager")
