/*
 * GlrShader.cpp
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */

#include <boost/regex.hpp>

#include "GlrShader.h"

#include "../../common/logger/Logger.h"

#include "GlrPreProcessor.h"

namespace glr {
namespace shaders {

GlrShader::GlrShader(std::string source, std::string baseDirectory) : source_(source), baseDirectory_(baseDirectory)
{
}

GlrShader::GlrShader(std::string name, std::string source, std::string baseDirectory) : name_(name), source_(source), baseDirectory_(baseDirectory)
{
}


GlrShader::~GlrShader()
{
}

void GlrShader::process(std::map< std::string, std::string > defineMap)
{
	LOG_DEBUG( "Processing shader '" + name_ + "'." );
	std::cout << "OK OK OK: " << name_ << " | " << baseDirectory_ << std::endl;
	GlrPreProcessor pp(source_, baseDirectory_);
	pp.process(defineMap);
	processedSource_ = pp.getProcessedSource();
	type_ = IShader::parseType(pp.getType());

	GlrParser op(processedSource_);
	op.parse();
	bindings_ = op.getBindings();

	// Remove all '@bind' lines
	const boost::regex e("@bind(\\s+)(\\S+)\\n+");
	processedSource_ = boost::regex_replace(processedSource_, e, "\n");
	//std::cout << "processedSource_: " << processedSource_ << std::endl;
}

std::string GlrShader::getName()
{
	return name_;
}

IShader::Type GlrShader::getType()
{
	return type_;
}

std::string GlrShader::getProcessedSource()
{
	return processedSource_;
}

std::string GlrShader::getSource()
{
	return source_;
}

GlrParser::StringBindingsMap GlrShader::getBindings()
{
	return bindings_;
}

bool GlrShader::containsPreProcessorCommands()
{
	// TODO: implement
	return true;
}
}
}
