/*
 * GlrShaderProgram.cpp
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */
#include "GlrShaderProgram.h"

#include "GlrPreProcessor.h"

#include "../../common/logging/Logger.h"

#include "../../exceptions/GlException.h"

namespace glr {
namespace shaders {
GlrShaderProgram::GlrShaderProgram(std::string source) : source_(source)
{
}

GlrShaderProgram::GlrShaderProgram(std::string name, std::string source) : name_(name), source_(source)
{
}


GlrShaderProgram::~GlrShaderProgram()
{
}

void GlrShaderProgram::process(std::map< std::string, std::shared_ptr<GlrShader> > glrShaderMap)
{
	LOG_DEBUG( "Processing shader program '" + name_ + "'." );

	// Pre-Process shaders
	GlrPreProcessor pp(source_);

	pp.process();

	name_ = pp.getName();
	LOG_DEBUG( "name: " + name_ );
	//LOG_DEBUG( << "getProcessedSource: " << pp.getProcessedSource();
	std::vector<CPreProcessor::ShaderData> shaders = pp.getShaders();

	std::stringstream msg;
	msg << "Initializing " << shaders.size() << " shaders.";
	LOG_DEBUG( msg.str() );

	for ( CPreProcessor::ShaderData s : shaders )
	{
		if ( glrShaderMap.find(s.name) != glrShaderMap.end())
		{
			// Found shader
			//shaders_[s.name] = GlrShader( s.name, glrShaderMap[s.name].getSource(), s.defineMap );
			//glrShaderMap[s.name].setType();

			shaders_.push_back(glrShaderMap[s.name]);
			shaders_.back()->process(s.defineMap);
		}
		else
		{
			LOG_ERROR( "Name requested: " + s.name );
			LOG_ERROR( "Names available: " );
			for ( auto s : glrShaderMap )
			{
				LOG_ERROR( s.first );
			}
			std::string msg("Could not find shader '" + name_ + "'.");
			LOG_ERROR( msg );
			throw exception::GlException(msg);
		}
	}
}

std::string GlrShaderProgram::getName()
{
	return name_;
}

std::vector< std::shared_ptr<GlrShader> > GlrShaderProgram::getShaders()
{
	return shaders_;
}
}
}
