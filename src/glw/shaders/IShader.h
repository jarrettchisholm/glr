/*
 * IShader.h
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#ifndef ISHADER_H_
#define ISHADER_H_

#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>

namespace glr {
namespace shaders {
class IShader {
public:
	enum Type {
		TYPE_NONE = 0,
		TYPE_VERTEX,
		TYPE_FRAGMENT,
		TYPE_GEOMETRY,
		TYPE_TESSELLATION
	};

	static Type parseType(std::string type)
	{
		if ( type.compare("fragment") == 0 )
			return TYPE_FRAGMENT;
		else if ( type.compare("vertex") == 0 )
			return TYPE_VERTEX;
		else if ( type.compare("geometry") == 0 )
			return TYPE_GEOMETRY;
		else if ( type.compare("tessellation") == 0 )
			return TYPE_TESSELLATION;

		return TYPE_NONE;
	}

	enum BindType {
		BIND_TYPE_NONE = 0,
		BIND_TYPE_LIGHT,
		BIND_TYPE_MATERIAL,
		BIND_TYPE_COLOR,
		BIND_TYPE_TEXTURE,
		BIND_TYPE_ANIMATION
	};

	static BindType parseBindType(std::string type)
	{
		if ( type.compare("Light") == 0 )
			return BIND_TYPE_LIGHT;
		else if ( type.compare("Material") == 0 )
			return BIND_TYPE_MATERIAL;
		else if ( type.compare("Color") == 0 )
			return BIND_TYPE_COLOR;
		else if ( type.compare("Texture") == 0 )
			return BIND_TYPE_TEXTURE;
		else if ( type.compare("Animation") == 0 )
			return BIND_TYPE_ANIMATION;

		return BIND_TYPE_NONE;
	}

	typedef std::pair<BindType, std::string> Binding;
	typedef std::vector< Binding > BindingsMap;


	virtual ~IShader()
	{
	}
	;


	virtual void bind() = 0;
	virtual Type getType() = 0;
	virtual GLuint getGLShaderId() = 0;
	virtual BindingsMap getBindings() = 0;
};
}
}
#endif /* ISHADER_H_ */
