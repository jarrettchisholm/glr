/*
 * IShader.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADER_H_
#define ISHADER_H_

#include <GL/glew.h>

namespace oglre {

namespace shaders {

class IShader {
public:
	typedef std::vector< std::pair<std::string, std::string> > BindingsMap;

	virtual ~IShader() {
	}
	;
	
	enum Type {
		TYPE_NONE = 0,
		TYPE_VERTEX,
		TYPE_FRAGMENT,
		TYPE_GEOMETRY,
		TYPE_TESSELLATION
	};
	
	static Type parseType(std::string type) {
		if (type.compare("fragment") == 0)
			return TYPE_FRAGMENT;
		else if (type.compare("vertex") == 0)
			return TYPE_VERTEX;
		else if (type.compare("geometry") == 0)
			return TYPE_GEOMETRY;
		else if (type.compare("tessellation") == 0)
			return TYPE_TESSELLATION;
			
		return TYPE_NONE;
	}
	
	virtual void bind() = 0;
	virtual Type getType() = 0;
	virtual GLuint getGLShaderId() = 0;
};

}

}

#endif /* ISHADER_H_ */
