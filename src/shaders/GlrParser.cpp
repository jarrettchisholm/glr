/*
 * GlrParser.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#include "GlrParser.h"

namespace glr {

namespace shaders {

GlrParser::GlrParser( std::string source ) : source_(source) {
}


GlrParser::~GlrParser() {
}

void GlrParser::parse() {
	parse(source_.begin(), source_.end());
}

template <typename It>
void GlrParser::parse(It f, It l) {
	grammar<It> p;

    StringBindingsMap mappings;
    bool ok = qi::phrase_parse(f, l, p, qi::blank, mappings);

    if (ok) {
		bindings_ = mappings;
        for (auto it = mappings.begin(); it!=mappings.end(); ++it)
            std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
    }

    if (f!=l)
        std::cerr << "warning: remaing unparsed: '" << std::string(f,l) << "'\n";
}

GlrParser::StringBindingsMap GlrParser::getBindings() {
	return bindings_;
}

}

}
