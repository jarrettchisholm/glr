/*
 * OglreShader.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */

#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include "OglreShader.h"

#include "OglrePreProcessor.h"

namespace oglre {

namespace shaders {

OglreShader::OglreShader(std::string source) : source_(source) {
}


OglreShader::~OglreShader() {	
}

void OglreShader::process(std::map< std::string, std::string > defineMap) {
	BOOST_LOG_TRIVIAL(debug) << "Processing shader.";
	OglrePreProcessor pp(source_);
	pp.process(defineMap);
	processedSource_ = pp.getProcessedSource();
	type_ = IShader::parseType( pp.getType() );
	
	OglreParser op(processedSource_);
	op.parse();
	bindings_ = op.getBindings();
	
	// Remove all '@bind' lines
	const boost::regex e("@bind(\\s+)(\\S+)\\n+");
    processedSource_ = boost::regex_replace(processedSource_, e, "\n");
    std::cout << "processedSource_: " << processedSource_ << std::endl;
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

OglreParser::StringBindingsMap OglreShader::getBindings() {
	return bindings_;
}

bool OglreShader::containsPreProcessorCommands() {
	// TODO: implement
	return true;
}

}

}
