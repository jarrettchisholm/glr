#ifndef GLRSHADER_H
#define GLRSHADER_H

#include <vector>
#include <map>
#include <string>

#include "IShader.hpp"

#include "GlrParser.hpp"

namespace glr
{
namespace shaders
{

/**
 * Class representing a 'glr' shader.  This is a shader that contains the 'raw' shader source (i.e. before it has been processed).
 * 
 * Note that, in the shader source, there are 2 kinds of #include directives.  The first one looks like this:
 * 
 * #include <lights>
 * 
 * The '<' and '>' around the filename indicate that this is a 'system' file to include.  GLR has several of these system shader source
 * files available for use.
 * 
 * The second one looks like this:
 * 
 * #include "myshader.vs"
 * 
 * This will include a file named 'myshader.vs' from the disk.  This is so that you can include your own, user-defined shader files.
 * 
 * You can use this class to then process the 'raw' shader source into something more amenable to an opengl shader.
 */
class GlrShader
{
public:
	GlrShader(std::string source, std::string baseDirectory = std::string());
	GlrShader(std::string name, std::string source, std::string baseDirectory = std::string());
	virtual ~GlrShader();

	void process(std::map< std::string, std::string > defineMap);

	std::string getName();
	IShader::Type getType();
	std::string getProcessedSource();
	std::string getSource();
	std::vector< std::pair<std::string, std::string> > getBindings();
	std::vector< std::pair<glmd::int32, std::string> > getLocationBindings();

	bool containsPreProcessorCommands();

private:
	std::string name_;
	IShader::Type type_;

	std::string source_;
	std::string processedSource_;
	std::string baseDirectory_;

	std::vector< std::pair<std::string, std::string> > bindBindings_;
	std::vector< std::pair<glmd::int32, std::string> > locationBindings_;
};

}
}

#endif /* GLRSHADER_H */
