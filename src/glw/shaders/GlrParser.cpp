/*
 * GlrParser.cpp
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */

#include <cstdlib>

#include <GL/glew.h>

#include "GlrParser.h"

#include "../../exceptions/Exception.h"
#include "../../exceptions/GlException.h"

#include "../../common/logger/Logger.h"

namespace glr {
namespace shaders {

GlrParser::GlrParser(std::string source) : source_(source)
{
}


GlrParser::~GlrParser()
{
}

void GlrParser::parse()
{
	parseBind(source_.begin(), source_.end());
	parseLocation(source_.begin(), source_.end());
}

template <typename It> void GlrParser::parseBind(It f, It l)
{
	// Parse @bind bindings
	{
		bindGrammar<It> g;
	
		std::vector< std::pair<std::string, std::string> > mappings;
		bool hasResults = qi::phrase_parse(f, l, g, qi::blank, mappings);
	
		if ( hasResults )
		{
			bindBindings_ = mappings;
			//for ( auto it = mappings.begin(); it != mappings.end(); ++it )
			//	std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
		}
	}

	//if (f!=l)
	//	std::cerr << "warning: remaing unparsed: '" << std::string(f,l) << "'\n";
}

template <typename It> void GlrParser::parseLocation(It f, It l)
{
	// Parse @location bindings
	{
		locationGrammar<It> g;
	
		std::vector< std::pair<std::string, std::string> > mappings;
		bool hasResults = qi::phrase_parse(f, l, g, qi::blank, mappings);
	
		if ( hasResults )
		{
			GLint maxNumLocations = 0;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxNumLocations);
			
			for ( auto& it : mappings )
			{
				auto p = std::pair<glmd::int32, std::string>();
				p.first = std::atoi(it.first.c_str());
				p.second = it.second;

				locationBindings_.push_back(p);
			}
			//for ( auto it = mappings.begin(); it != mappings.end(); ++it )
			//	std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
		}
	}
}

std::vector< std::pair<std::string, std::string> > GlrParser::getBindBindings()
{
	return bindBindings_;
}

std::vector< std::pair<glmd::int32, std::string> > GlrParser::getLocationBindings()
{
	return locationBindings_;
}

}
}
