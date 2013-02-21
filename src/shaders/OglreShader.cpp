/*
 * OglreShader.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#include "OglreShader.h"

#include "OglrePreProcessor.h"
#include "OglreParser.h"

namespace oglre {

namespace shaders {

OglreShader::OglreShader(std::string source) : source_(source) {
}


OglreShader::~OglreShader() {	
}

void OglreShader::process(std::map< std::string, std::string > defineMap) {
	OglrePreProcessor pp(source_);
	pp.process(defineMap);
	processedSource_ = pp.getProcessedSource();
	
	OglreParser op(processedSource_);
	op.parse();
	bindings_ = op.getBindings();
}

std::string OglreShader::getName() {
	return name_;
}

IShader::Type OglreShader::getType() {
	return type_;
}

std::string OglreShader::getProcessedSource() {
	return processedSource_;
}

std::string OglreShader::getSource() {
	return source_;
}

OglreShader::BindingsMap OglreShader::getBindings() {
	return bindings_;
}

bool OglreShader::containsPreProcessorCommands() {
	// TODO: implement
	return true;
}

}

}
