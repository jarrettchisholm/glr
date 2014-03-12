#include <utility>

#include "glw/shaders/GlrShaderProgram.hpp"

#include "glw/shaders/GlrPreProcessor.hpp"

#include "common/logger/Logger.hpp"

#include "exceptions/GlException.hpp"

namespace glr
{
namespace shaders
{

GlrShaderProgram::GlrShaderProgram(std::string source, std::string baseDirectory) : source_(std::move(source)), baseDirectory_(std::move(baseDirectory))
{
}

GlrShaderProgram::GlrShaderProgram(std::string name, std::string source, std::string baseDirectory) : name_(std::move(name)), source_(std::move(source)), baseDirectory_(std::move(baseDirectory))
{
}


GlrShaderProgram::~GlrShaderProgram()
{
}

void GlrShaderProgram::process(const std::map< std::string, std::unique_ptr<GlrShader> >& glrShaderMap)
{
	LOG_DEBUG( "Processing shader program '" + name_ + "'." );

	// Pre-Process shaders
	GlrPreProcessor pp(source_, baseDirectory_);

	pp.process();

	name_ = pp.getName();
	LOG_DEBUG( "name: " + name_ );
	//LOG_DEBUG( << "getProcessedSource: " << pp.getProcessedSource();
	std::vector<CPreProcessor::ShaderData> shaders = pp.getShaders();

	LOG_DEBUG( "Initializing " << shaders.size() << " shaders." );

	for ( CPreProcessor::ShaderData s : shaders )
	{
		auto it = glrShaderMap.find(s.name);
		if ( it != glrShaderMap.end() )
		{
			// Found shader
			//shaders_[s.name] = GlrShader( s.name, glrShaderMap[s.name].getSource(), s.defineMap );
			//glrShaderMap[s.name].setType();

			shaders_.push_back(it->second.get());
			shaders_.back()->process(s.defineMap);
		}
		else
		{
			LOG_ERROR( "Name requested: " + s.name );
			LOG_ERROR( "Names available: " );
			for ( auto& s : glrShaderMap )
			{
				LOG_ERROR( s.first );
			}
			
			std::string msg("Could not find shader '" + name_ + "'.");
			LOG_ERROR( msg );
			throw exception::GlException(msg);
		}
	}
}

const std::string& GlrShaderProgram::getName() const
{
	return name_;
}

std::vector< GlrShader* > GlrShaderProgram::getShaders()
{
	return shaders_;
}

}
}
