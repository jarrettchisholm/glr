/*
 * OglreShaderProgram.h
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#ifndef OGLRESHADERPROGRAM_H
#define OGLRESHADERPROGRAM_H

#include <memory>

#include "OglreShader.h"

namespace oglre {

namespace shaders {
	
class OglreShaderProgram {
public:
	OglreShaderProgram(std::string source);
	virtual ~OglreShaderProgram();
	
	void process(std::map< std::string, std::shared_ptr<OglreShader> > oglreShaderMap);
	
	std::string getName();
	
	std::vector< std::shared_ptr<OglreShader> > getShaders();
	
private:
	std::string name_;
	std::string source_;
	
	std::vector< std::shared_ptr<OglreShader> > shaders_;
};

}

}
#endif /* OGLRESHADERPROGRAM_H */ 
