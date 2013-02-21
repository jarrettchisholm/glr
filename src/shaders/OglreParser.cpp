/*
 * OglreParser.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#include "OglreParser.h"

namespace oglre {

namespace shaders {

OglreParser::OglreParser( std::string source ) : source_(source) {
}


OglreParser::~OglreParser() {
}

void OglreParser::parse() {
	parse(source.begin(), source.end());
}

template <typename It>
void OglreParser::parse(It f, It l) {
	grammar<It> p;

    Map mappings;
    bool ok = qi::phrase_parse(f, l, p, qi::blank, mappings);

    if (ok) {
		variableBindings_ = mappings;
        for (auto it = mappings.begin(); it!=mappings.end(); ++it)
            std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
    }

    if (f!=l)
        std::cerr << "warning: remaing unparsed: '" << std::string(f,l) << "'\n";
}

}

}
