#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IShader.hpp"

namespace glmd = glm::detail;

namespace glr
{
namespace shaders
{

/**
 * Class representing a 'glsl' shader.  This is a shader that contains the final, processed shader source.
 */
class GlslShader : public IShader
{
public:
	GlslShader(std::string source, Type type);
	GlslShader(std::string name, std::string source, Type type, std::vector< std::pair<std::string, std::string> > bindings, std::vector< std::pair<glmd::int32, std::string> > locationBindings);
	virtual ~GlslShader();

	virtual void bind();
	virtual IShader::Type getType();
	virtual GLuint getGLShaderId();
	virtual IShader::BindingsMap getBindings();

	void compile();

	std::string getName();

private:
	GLuint shaderId_;

	std::string name_;
	std::string source_;
	Type type_;

	BindingsMap bindings_;
};

}
}

#endif /* GLSLSHADER_H_ */
