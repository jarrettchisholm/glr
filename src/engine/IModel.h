/*
 * IModel.h
 *
 *  Created on: 2012-10-20
 *      Author: jarrett
 */

#ifndef IMODEL_H_
#define IMODEL_H_

namespace icee {

namespace engine {

class IModel {
public:
	IModel() {
	}
	;
	
	virtual ~IModel() {
	}
	;
	
	virtual void render() = 0;
	

};

}

}

#endif /* IMODEL_H_ */
