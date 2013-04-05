/*
 * GlrShader.h
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */


#ifndef GLRSHADER_H
#define GLRSHADER_H

#include <vector>
#include <map>
#include <string>

#include "IShader.h"

#include "GlrParser.h"

namespace glr {
namespace shaders {
class GlrShader {
public:
	GlrShader(std::string source);
	virtual ~GlrShader();

	void process(std::map< std::string, std::string > defineMap);

	std::string getName();
	IShader::Type getType();
	std::string getProcessedSource();
	std::string getSource();
	GlrParser::StringBindingsMap getBindings();

	bool containsPreProcessorCommands();

private:
	std::string name_;
	IShader::Type type_;

	std::string source_;
	std::string processedSource_;

	GlrParser::StringBindingsMap bindings_;
};
}
}
#endif /* GLRSHADER_H */
