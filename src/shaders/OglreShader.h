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

namespace oglre {

namespace shaders {

class OglreShader {
	typedef std::vector< std::pair<std::string, std::string> > BindingsMap;
	
public:
	OglreShader();
	virtual ~OglreShader();
	
	void process(std::map< std::string, std::string > defineMap);
	
	std::string getName();
	IShader::Type getType();
	std::string getProcessedSource();
	std::string getSource();
	BindingsMap getVariableBindings();
	
private:
	std::string name_;
	IShader::Type type_;
	
	std::string sourceCode_;
	std::string processedSourceCode_;
	
	BindingsMap variableBindings_;
};

}

}
#endif /* OGLRESHADER_H */ 
