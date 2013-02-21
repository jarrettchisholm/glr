/*
 * OglreShaderProgram.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#include "OglreShaderProgram.h"

#include "OglrePreProcessor.h"

namespace oglre {

namespace shaders {

OglreShaderProgram::OglreShaderProgram(std::string source) : source_(source) {
}


OglreShaderProgram::~OglreShaderProgram() {	
}

void OglreShaderProgram::process(std::map< std::string, std::shared_ptr<OglreShader> > oglreShaderMap) {	
	// Pre-Process shaders
	OglrePreProcessor pp(source_);
	
	pp.process();
	
	name_ = pp.getName();
	
	std::vector<CPreProcessor::ShaderData> shaders = pp.getShaders();
	
	for ( CPreProcessor::ShaderData s : shaders) {
		if ( oglreShaderMap.find(s.name) != oglreShaderMap.end() ) {
			// Found shader
			//shaders_[s.name] = OglreShader( s.name, oglreShaderMap[s.name].getSource(), s.defineMap );
			shaders_.push_back( oglreShaderMap[s.name] );
			shaders_.back()->process( s.defineMap );
		} else {
			// TODO: ERROR
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
