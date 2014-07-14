#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include <memory>
#include <string>
#include <map>
#include <mutex>

#include <GL/glew.h>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "models/IModelManager.hpp"

#include "IdManager.hpp"

#include "serialize/SplitMember.hpp"

namespace glr
{

namespace glw
{
class IOpenGlDevice;
}

namespace models
{

class ModelLoader;
class Model;

class ModelManager : public IModelManager
{
public:
	ModelManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~ModelManager();

	virtual IModel* getModelTemplate(Id id) const;
	virtual IModel* getModelTemplate(const std::string& name) const;

	virtual void loadModel(const std::string& name, const std::string& filename, bool initialize = true);

	virtual void destroyModelTemplate(Id id);
	virtual void destroyModelTemplate(const std::string& name);
	virtual void destroyModelTemplate(IModel* model);

	virtual IModel* createInstance(const std::string& name);

	virtual void destroyInstance(Id id);
	virtual void destroyInstance(IModel* model);

	virtual IModel* getInstance(Id id) const;
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);

private:	
	/**
	 * Required for serialization.
	 */
	ModelManager();

	glw::IOpenGlDevice* openGlDevice_;

	aiLogStream stream;

	std::vector< std::unique_ptr<Model> > models_;
	std::vector< std::unique_ptr<IModel> > modelInstances_;
	
	std::unique_ptr<ModelLoader> modelLoader_;
	
	IdManager idManager_;
	
	mutable std::recursive_mutex accessMutex_;
	
	Model* getModel(Id id) const;
	Model* getModel(const std::string& name) const;
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
	// Need to do these because boost serialization doesn't have a standard implementation for std::unique_ptr
	// Apparently, std::unique_ptr will have a serializable implementation in boost 1.56
	// TODO: Implement one myself?
	template<class Archive> void save(Archive & ar, const unsigned int version) const;
	template<class Archive> void load(Archive & ar, const unsigned int version);
};

}
}

#endif /* MODELMANAGER_H_ */
