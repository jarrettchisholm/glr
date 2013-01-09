/*
 * IIShaderProgram.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADERPROGRAM_H_
#define ISHADERPROGRAM_H_

namespace oglre {

namespace shaders {

class IShaderProgram {
public:
	virtual ~IShaderProgram() {
	}
	;
	
	virtual void bind() = 0;
};

}

}

#endif /* ISHADERPROGRAM_H_ */
