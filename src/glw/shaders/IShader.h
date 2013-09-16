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

#include <iostream>

#include <GL/glew.h>

namespace glr {
namespace shaders {

class IShader {
public:
	/**
	 * The different types of shaders available.
	 */
	enum Type {
		TYPE_NONE = 0,
		TYPE_VERTEX,
		TYPE_FRAGMENT,
		TYPE_GEOMETRY,
		TYPE_TESSELLATION
	};

	/**
	 * Parses the type of shader of the given string type, and returns the shader type.
	 * 
	 * @param type The type of shader, as a string.
	 * 
	 * @return The shader type.
	 */
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

	/**
	 * The different types of data that can be bound to a shader from glr.
	 * This data is bound in the shader using a tag, such as '@Light'.  When the shader is parsed, the
	 * name of the variable that the '@Light' binding is associated with is stored.  Later, when we bind a shader
	 * and want to bind, for example, light data, we can reference this stored shader variable name, and bind our
	 * light data to it.  This way, we don't have to hard code any shader variable names.
	 */
	enum BindType {
		BIND_TYPE_NONE = 0,
		BIND_TYPE_LIGHT,
		BIND_TYPE_MATERIAL,
		BIND_TYPE_COLOR,
		BIND_TYPE_TEXTURE,
		BIND_TYPE_BONE
	};

	/**
	 * Parses the type of shader data of the given string type, and returns the shader data type.
	 * 
	 * @param type The data type of a shader variable, as a string.
	 * 
	 * @return The shader data type.
	 */
	static BindType parseBindType(std::string type)
	{
		std::cout << type << std::endl;
		if ( type.compare("Light") == 0 )
			return BIND_TYPE_LIGHT;
		else if ( type.compare("Material") == 0 )
			return BIND_TYPE_MATERIAL;
		else if ( type.compare("Color") == 0 )
			return BIND_TYPE_COLOR;
		else if ( type.compare("Texture") == 0 )
			return BIND_TYPE_TEXTURE;
		else if ( type.compare("Bone") == 0 )
			return BIND_TYPE_BONE;

		return BIND_TYPE_NONE;
	}

	typedef std::pair<BindType, std::string> Binding;
	typedef std::vector< Binding > BindingsMap;


	virtual ~IShader()
	{
	}
	;


	/**
	 * Binds the current IShader object to OpenGL.  This is used when compiling
	 * a shader program.
	 */
	virtual void bind() = 0;
	virtual Type getType() = 0;
	virtual GLuint getGLShaderId() = 0;
	
	/**
	 * Gets the bindings for this IShader object.
	 * 
	 * @return The bindings for this IShader object.
	 */
	virtual BindingsMap getBindings() = 0;
};
}
}
#endif /* ISHADER_H_ */
