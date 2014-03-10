#ifndef ISHADER_H_
#define ISHADER_H_

#include <utility>
#include <string>
#include <vector>
#include <map>

#include <iostream>

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace glmd = glm::detail;

namespace glr
{
namespace shaders
{

// Reserved vertex attribute locations
const static std::vector< std::pair< std::string, glmd::uint32> > reservedVertexAttribLocations = {
	std::pair< std::string, glmd::uint32>( std::string("in_Position"), 		0),
	std::pair< std::string, glmd::uint32>( std::string("in_Texture"), 		1),
	std::pair< std::string, glmd::uint32>( std::string("in_Normal"), 		2),
	std::pair< std::string, glmd::uint32>( std::string("in_Color"), 		3),
	std::pair< std::string, glmd::uint32>( std::string("in_BoneIds"), 		4),
	std::pair< std::string, glmd::uint32>( std::string("in_BoneWeights"), 	5)
};

class IShader
{
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
		BIND_TYPE_LOCATION,
		BIND_TYPE_LIGHT,
		BIND_TYPE_MATERIAL,
		BIND_TYPE_COLOR,
		BIND_TYPE_TEXTURE_1D,
		BIND_TYPE_TEXTURE_1D_ARRAY,
		BIND_TYPE_TEXTURE_2D,
		BIND_TYPE_TEXTURE_2D_ARRAY,
		BIND_TYPE_TEXTURE_3D,
		BIND_TYPE_BONE
	};


	// TODO: Maybe switch to using a map for mapping bindings?
	/*
	const static std::map<std::string, BindType> bindMap = {
		{ std::string("Light"), 			BIND_TYPE_LIGHT }, 
		{ std::string("Material"), 			BIND_TYPE_MATERIAL }, 
		{ std::string("Color"), 			BIND_TYPE_COLOR }, 
		{ std::string("Texture1D"), 		BIND_TYPE_TEXTURE_1D },
		{ std::string("Texture1DArray"), 	BIND_TYPE_TEXTURE_1D_ARRAY },
		{ std::string("Texture"), 			BIND_TYPE_TEXTURE },
		{ std::string("TextureArray"), 		BIND_TYPE_TEXTURE_ARRAY },
		{ std::string("Texture3D"), 		BIND_TYPE_TEXTURE_3D },
		{ std::string("Bone"), 				BIND_TYPE_BONE }
	};
	*/
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
		else if ( type.compare("Texture1D") == 0 )
			return BIND_TYPE_TEXTURE_1D;
		else if ( type.compare("Texture1DArray") == 0 )
			return BIND_TYPE_TEXTURE_1D_ARRAY;
		else if ( type.compare("Texture2D") == 0 )
			return BIND_TYPE_TEXTURE_2D;
		else if ( type.compare("Texture2DArray") == 0 )
			return BIND_TYPE_TEXTURE_2D_ARRAY;
		else if ( type.compare("Texture3D") == 0 )
			return BIND_TYPE_TEXTURE_3D;
		else if ( type.compare("Bone") == 0 )
			return BIND_TYPE_BONE;

		return BIND_TYPE_NONE;
	}

	struct Binding
	{
		BindType type;
		std::string variableName;
		GLuint bindPoint;
	};
	//typedef std::pair<BindType, std::string> Binding;
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
	virtual Type getType() const = 0;
	virtual GLuint getGLShaderId() const = 0;
	
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
