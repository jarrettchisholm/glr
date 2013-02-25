/*
 * OglreShaderProgram.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */

#include <boost/log/trivial.hpp>

#include "OglreShaderProgram.h"

#include "OglrePreProcessor.h"

#include "../exceptions/GlException.h"

namespace oglre {

namespace shaders {

OglreShaderProgram::OglreShaderProgram(std::string source) : source_(source) {
}


OglreShaderProgram::~OglreShaderProgram() {	
}

void OglreShaderProgram::process(std::map< std::string, std::shared_ptr<OglreShader> > oglreShaderMap) {	
	BOOST_LOG_TRIVIAL(debug) << "Processing shader program.";
	
	// Pre-Process shaders
	OglrePreProcessor pp(source_);
	
	pp.process();
	
	name_ = pp.getName();
	BOOST_LOG_TRIVIAL(debug) << "name: " << name_;
	//BOOST_LOG_TRIVIAL(debug) << "getProcessedSource: " << pp.getProcessedSource();
	std::vector<CPreProcessor::ShaderData> shaders = pp.getShaders();
	
	BOOST_LOG_TRIVIAL(debug) << "Initializing shaders: " << shaders.size();
	
	for ( CPreProcessor::ShaderData s : shaders) {
		if ( oglreShaderMap.find(s.name) != oglreShaderMap.end() ) {
			// Found shader
			//shaders_[s.name] = OglreShader( s.name, oglreShaderMap[s.name].getSource(), s.defineMap );
			//oglreShaderMap[s.name].setType();
			
			shaders_.push_back( oglreShaderMap[s.name] );
			shaders_.back()->process( s.defineMap );
		} else {
			BOOST_LOG_TRIVIAL(error) << "Name requested: " << s.name;
			BOOST_LOG_TRIVIAL(error) << "Names available: ";
			for ( auto s : oglreShaderMap) {
				BOOST_LOG_TRIVIAL(error) << s.first;
			}
			std::string msg("Could not find shader.");
			BOOST_LOG_TRIVIAL(error) << msg;
			throw exception::GlException(msg);
		}
	}
}

std::string OglreShaderProgram::getName() {
	return name_;
}

std::vector< std::shared_ptr<OglreShader> > OglreShaderProgram::getShaders() {
	return shaders_;
}

}

}
