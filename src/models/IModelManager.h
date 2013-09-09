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

namespace glr {
namespace models {
class IModelManager {
public:
	virtual ~IModelManager()
	{
	}
	;

	virtual IModel* getModel(const std::string filename) = 0;
	virtual void loadModel(const std::string filename) = 0;
	virtual std::unique_ptr<IModel> createModel(const std::string filename) = 0;
};
}
}
#endif /* IMODELMANAGER_H_ */
