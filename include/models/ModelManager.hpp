#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include <GL/glew.h>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "models/IModelManager.hpp"

#include "models/ModelLoader.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "models/Model.hpp"

#include "IdManager.hpp"

namespace glr
{
namespace models
{

class ModelManager : public IModelManager
{
public:
	ModelManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~ModelManager();

	virtual IModel* getModelTemplate(Id id) const;
	virtual IModel* getModelTemplate(const std::string& name) const;

	virtual void loadModel(const std::string& name, const std::string& filename);

	virtual void destroyModelTemplate(Id id);
	virtual void destroyModelTemplate(const std::string& name);
	virtual void destroyModelTemplate(IModel* model);

	virtual IModel* createInstance(const std::string& name);

	virtual void destroyInstance(Id id);
	virtual void destroyInstance(IModel* model);

	virtual IModel* getInstance(Id id) const;

private:	
	glw::IOpenGlDevice* openGlDevice_;

	aiLogStream stream;

	std::vector< std::unique_ptr<Model> > models_;
	std::vector< std::unique_ptr<IModel> > modelInstances_;
	
	std::unique_ptr<ModelLoader> modelLoader_;
	
	IdManager idManager_;
	
	Model* getModel(Id id) const;
	Model* getModel(const std::string& name) const;
};

}
}

#endif /* MODELMANAGER_H_ */
