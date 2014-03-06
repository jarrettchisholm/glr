#ifndef IMODELMANAGER_H_
#define IMODELMANAGER_H_

#include <string>

#include "IModel.hpp"

#include "glw/Mesh.hpp"

namespace glr
{
namespace models
{

class IModelManager
{
public:
	virtual ~IModelManager()
	{
	}
	;

	/**
	 *
	 * @return The raw pointer to a Model object.  The caller does not own the pointer - it is managed by
	 * the ModelManager.
	 */
	virtual IModel* getModelTemplate(glm::detail::uint32 id) = 0;
	virtual IModel* getModelTemplate(const std::string& name) = 0;
	
	/**
	 * Loads the model from the given filename.
	 */
	virtual void loadModel(const std::string& name, const std::string& filename) = 0;
	
	virtual void destroyModelTemplate(glm::detail::uint32 id) = 0;
	virtual void destroyModelTemplate(const std::string& name) = 0;
	virtual void destroyModelTemplate(IModel* model) = 0;
	
	/**
	 * Creates an instance of the model with name 'name'.  This model can have animations and animation
	 * times set distinctly from other models created with this method.
	 * 
	 * @return A unique pointer to a model object.  The model is a copy of the model that was previously loaded
	 * with the specified name.
	 */
	virtual IModel* createInstance(const std::string& name) = 0;
	
	virtual void destroyInstance(glm::detail::uint32 id) = 0;
	virtual void destroyInstance(IModel* model) = 0;
	
	virtual IModel* getInstance(glm::detail::uint32 id) = 0;
};

}
}

#endif /* IMODELMANAGER_H_ */
