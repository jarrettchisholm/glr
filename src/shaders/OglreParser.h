/*
 * OglreParser.h
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#ifndef OGLREPARSER_H
#define OGLREPARSER_H

#define BOOST_SPIRIT_DEBUG
#include <iostream>
#include <fstream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted.hpp>
#include <map>

namespace qi = boost::spirit::qi;

namespace oglre {

namespace shaders {

class OglreParser {
	typedef std::vector< std::pair<std::string, std::string> > Map;

public:
	OglreParser(std::string source);
	virtual ~OglreParser();
	
	void parse();
	
	Map getBindings();


private:
	std::string source_;
	Map bindings_;
	
	template <typename It>
	void parse(It f, It l);
	
	template <typename It>
	struct grammar : qi::grammar<It, Map(), qi::blank_type> {
			
		public:
			grammar() : grammar::base_type(start) {
				using namespace qi;
				ident = lexeme [ alpha >> *alnum ];
				
				annot = "@bind" >> ident >> eol;
				
				declaration = 
					( 
						omit [ +(ident >> !char_(';')) ] // omit the type, TODO
						>> ident >> ';' >> eol
					)
					| // or
					(
						omit [ ident ] >> ident >> '{' >> *eol
						>> omit [ +(ident >> !char_('[')) ]
						>> ident >> '[' >> ignore
					);
				
				ignore = !annot >> *(char_ - eol) >> eol;
				
				combi = annot >> declaration;
				start = *ignore >> combi % *ignore;
				
				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(combi);
				BOOST_SPIRIT_DEBUG_NODE(ignore);
				BOOST_SPIRIT_DEBUG_NODE(declaration);
				BOOST_SPIRIT_DEBUG_NODE(annot);
				BOOST_SPIRIT_DEBUG_NODE(ident);
			}
			
		private:
			qi::rule<It, qi::blank_type> ignore;
			qi::rule<It, std::string(), qi::blank_type> ident, declaration, annot;
			qi::rule<It, std::pair<std::string, std::string>(), qi::blank_type> combi;
			qi::rule<It, Map(), qi::blank_type> start;
		};
		
};

}

}

#endif /* OGLREPARSER_H */ 
