/*
 * IModelManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IMODELMANAGER_H_
#define IMODELMANAGER_H_

#include <string>

#include "IModel.h"

#include "../glw/Mesh.h"

namespace glr {
namespace models {

class IModelManager {
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
	virtual IModel* getModel(const std::string& name) = 0;
	
	/**
	 * Loads the model from the given filename.
	 */
	virtual void loadModel(const std::string& name, const std::string& filename) = 0;
	
	/**
	 * Creates an instance of the model with name 'name'.  This model can have animations and animation
	 * times set distinctly from other models created with this method.
	 * 
	 * @return A unique pointer to a model object.  The model is a copy of the model that was previously loaded
	 * with the specified name.
	 */
	virtual std::unique_ptr<IModel> createInstance(const std::string& name) = 0;
};

}
}
#endif /* IMODELMANAGER_H_ */
