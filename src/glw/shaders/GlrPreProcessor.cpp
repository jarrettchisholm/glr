#include "GlrPreProcessor.h"

#include "ShaderData.h"

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
