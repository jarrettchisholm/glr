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

	virtual IModel* getModel(const std::string& name) = 0;
	virtual void loadModel(const std::string& name, const std::string& filename) = 0;
	virtual std::unique_ptr<IModel> createModel(const std::string& name) = 0;
	
	// TESTING
	virtual std::unique_ptr<IModel> createModel(glw::Mesh* mesh) = 0;
};

}
}
#endif /* IMODELMANAGER_H_ */
