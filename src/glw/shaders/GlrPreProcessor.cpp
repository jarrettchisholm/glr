#include "glw/shaders/GlrPreProcessor.hpp"

#include "glw/shaders/ShaderData.hpp"

namespace glr
{
namespace shaders
{

GlrPreProcessor::GlrPreProcessor(std::string source, std::string baseDirectory) : CPreProcessor(source, baseDirectory)
{
	CPreProcessor::files_ = SHADER_DATA;
}

}
}
