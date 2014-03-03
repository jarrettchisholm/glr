#ifndef GLRSHADERPROGRAM_H
#define GLRSHADERPROGRAM_H

#include <memory>

#include "GlrShader.hpp"

namespace glr
{
namespace shaders
{
	
/**
 * Class representing a 'glr' shader program.  This is source data that contains #defines, #includes, etc.  However, unlike actual shader source files,
 * the source for a shader program doesn't actually include source data when processing the #include directive.  Instead, this directive indicates
 * a shader source file that is to be used when 'building/compiling' this shader program.
 * 
 * The #include directive indicates a shader source file to use for this shader program.  During processing of the GlrShaderProgram, it will
 * load the GlrShader specified, and also pass any #defines, etc, that have been set before the #include directive.  This will
 * allow you to specify some #define values before actually processing a source file, which can be useful if you want a shader program to
 * have, for example, a certain number of lights available use.
 * 
 * Note that GlrShader objects are reused between different GlrShaderPrograms.  This means that, once you have processed a GlrShaderProgram,
 * you will need to convert it into a GlslShaderProgram immediately after.  Otherwise, the processing you have done using the GlrShader objects
 * may be lost once they are reused in processing of another GlrShaderProgram.
 */
class GlrShaderProgram
{
public:
	GlrShaderProgram(std::string source, std::string baseDirectory = std::string());
	GlrShaderProgram(std::string name, std::string source, std::string baseDirectory = std::string());
	virtual ~GlrShaderProgram();

	/**
	 * Process this shader program.  This function will find the shaders that are required for this shader program using the provided
	 * glrShaderMap, and use those shaders (i.e. process those shaders) for this particular shader program.
	 * 
	 * @param glrShaderMap A map that maps a shader filename (as a string) to the GlrShader object.
	 */
	void process(std::map< std::string, std::shared_ptr<GlrShader> > glrShaderMap);

	virtual std::string getName();

	std::vector< std::shared_ptr<GlrShader> > getShaders();

private:
	std::string name_;
	std::string source_;

	std::vector< std::shared_ptr<GlrShader> > shaders_;
	std::string baseDirectory_;
};

}
}

#endif /* GLRSHADERPROGRAM_H */
