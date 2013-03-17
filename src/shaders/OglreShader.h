/*
 * OglreShader.h
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#ifndef OGLRESHADER_H
#define OGLRESHADER_H

#include <vector>
#include <map>
#include <string>

#include "IShader.h"

#include "OglreParser.h"

namespace oglre {

namespace shaders {

class OglreShader {
public:
	OglreShader(std::string source);
	virtual ~OglreShader();
	
	void process(std::map< std::string, std::string > defineMap);
	
	std::string getName();
	IShader::Type getType();
	std::string getProcessedSource();
	std::string getSource();
	OglreParser::StringBindingsMap getBindings();
	
	bool containsPreProcessorCommands();
	
private:
	std::string name_;
	IShader::Type type_;
	
	std::string source_;
	std::string processedSource_;
	
	OglreParser::StringBindingsMap bindings_;
};

}

}
#endif /* OGLRESHADER_H */ 
