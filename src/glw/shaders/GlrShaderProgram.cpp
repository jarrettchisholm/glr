/*
 * GlrShaderProgram.cpp
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */

#include <boost/log/trivial.hpp>

#include "GlrShaderProgram.h"

#include "GlrPreProcessor.h"

#include "../../exceptions/GlException.h"

namespace glr {
namespace shaders {
GlrShaderProgram::GlrShaderProgram(std::string source) : source_(source)
{
}


GlrShaderProgram::~GlrShaderProgram()
{
}

void GlrShaderProgram::process(std::map< std::string, std::shared_ptr<GlrShader> > glrShaderMap)
{
	BOOST_LOG_TRIVIAL(debug) << "Processing shader program.";

	// Pre-Process shaders
	GlrPreProcessor pp(source_);

	pp.process();

	name_ = pp.getName();
	BOOST_LOG_TRIVIAL(debug) << "name: " << name_;
	//BOOST_LOG_TRIVIAL(debug) << "getProcessedSource: " << pp.getProcessedSource();
	std::vector<CPreProcessor::ShaderData> shaders = pp.getShaders();

	BOOST_LOG_TRIVIAL(debug) << "Initializing shaders: " << shaders.size();

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
			BOOST_LOG_TRIVIAL(error) << "Name requested: " << s.name;
			BOOST_LOG_TRIVIAL(error) << "Names available: ";
			for ( auto s : glrShaderMap )
			{
				BOOST_LOG_TRIVIAL(error) << s.first;
			}
			std::string msg("Could not find shader.");
			BOOST_LOG_TRIVIAL(error) << msg;
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
