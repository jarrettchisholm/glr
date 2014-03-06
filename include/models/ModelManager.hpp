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

namespace glr
{
namespace models
{

class ModelManager : public IModelManager
{
public:
	ModelManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~ModelManager();

	
	/**
	 *
	 * @return The raw pointer to a Model object.  The caller does not own the pointer - it is managed by
	 * the ModelManager.
	 */
	virtual IModel* getModelTemplate(glm::detail::uint32 id);
	virtual IModel* getModelTemplate(const std::string& name);
	
	/**
	 * Loads the model from the given filename.
	 */
	virtual void loadModel(const std::string& name, const std::string& filename);
	
	virtual void destroyModelTemplate(glm::detail::uint32 id);
	virtual void destroyModelTemplate(const std::string& name);
	virtual void destroyModelTemplate(IModel* model);
	
	/**
	 * Creates an instance of the model with name 'name'.  This model can have animations and animation
	 * times set distinctly from other models created with this method.
	 * 
	 * @return A unique pointer to a model object.  The model is a copy of the model that was previously loaded
	 * with the specified name.
	 */
	virtual IModel* createInstance(const std::string& name);
	
	virtual void destroyInstance(glm::detail::uint32 id);
	virtual void destroyInstance(IModel* model);
	
	virtual IModel* getInstance(glm::detail::uint32 id);

private:	
	glw::IOpenGlDevice* openGlDevice_;

	aiLogStream stream;

	std::map< std::string, std::unique_ptr<Model> > models_;
	std::vector< std::unique_ptr<IModel> > modelInstances_;
	
	std::unique_ptr<ModelLoader> modelLoader_;
};

}
}

#endif /* MODELMANAGER_H_ */
