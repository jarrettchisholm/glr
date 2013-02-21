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

OglreShader::OglreShader() {
}


OglreShader::~OglreShader() {	
}

void OglreShader::process(std::map< std::string, std::string > defineMap) {
	OglrePreProcessor pp(sourceCode_);
	pp.process(defineMap);
	processedSourceCode_ = pp.getProcessedSource();
	
	OglreParser op(processedSourceCode_);
	op.parse();
	variableBindings_ = op.getVariableBindings();
}

std::string OglreShader::getName() {
	return name_;
}

IShader::Type OglreShader::getType() {
	return type_;
}

std::string OglreShader::getProcessedSource() {
	return processedSourceCode_;
}

std::string OglreShader::getSource() {
	return sourceCode_;
}

BindingsMap OglreShader::getVariableBindings() {
	return variableBindings_;
}

}

}
