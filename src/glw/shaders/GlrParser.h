/*
 * GlrParser.h
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */


#ifndef GLRPARSER_H
#define GLRPARSER_H

#define BOOST_SPIRIT_DEBUG
#include <iostream>
#include <fstream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted.hpp>
#include <map>

#include <glm/glm.hpp>

namespace qi = boost::spirit::qi;
namespace glmd = glm::detail;

namespace glr {
namespace shaders {

class GlrParser {
	typedef std::vector< std::pair<std::string, std::string> > Map;

public:
	typedef std::vector< std::pair<std::string, std::string> > StringBindingsMap;
	typedef std::vector< std::pair<glmd::int32, std::string> > IntegerBindingsMap;

	GlrParser(std::string source);
	virtual ~GlrParser();

	void parse();

	StringBindingsMap getBindBindings();
	IntegerBindingsMap getLocationBindings();


	/*
	   (
	        omit [ ident ] >> ident >> '{' >> *eol
	        >> omit [ +(ident >> !char_('[')) ]
	        >> ident >> '[' >> ignore
	   );
	 */

private:
	std::string source_;
	StringBindingsMap bindBindings_;
	IntegerBindingsMap locationBindings_;

	template <typename It> void parseBind(It f, It l);
	template <typename It> void parseLocation(It f, It l);

	template <typename It> struct bindGrammar : qi::grammar<It, Map(), qi::blank_type> {
public:
		bindGrammar() : bindGrammar::base_type(start)
		{
			using namespace qi;
			ident = lexeme [ alpha >> *alnum ];

			annot = "@bind" >> ident >> eol;

			layout = "layout(" >> *(omit [ ident ]) >> ')';

			declaration =

			    /*
			     * Match to things like:
			     * @bind texture0
			     * uniform sampler2D texture;
			     *
			     * or
			     *
			     * @bind texture0
			     * uniform
			     * sampler2D
			     * texture
			     * ;
			     */
				(
					-layout >> *eol
					>> omit [ +((ident | eol) >> (*eol >> !char_(';'))) ] >> *eol
					>> ident >> *eol
					>> ';' >> eol
				)

				// or
				|

			    /*
			     * Match to things like:
			     * @bind Light
			     * layout(std140) uniform LightSources {
			     *  LightSource lightSources[ NUM_LIGHTS ];
			     * };
			     *
			     * or
			     *
			     * @bind Light
			     * layout(std140)
			     * uniform
			     * LightSources
			     * {
			     *  LightSource lightSources[ NUM_LIGHTS ];
			     * };
			     *
			     * or
			     *
			     * @bind Light2
			     * uniform LightSources3
			     * {
			     *  LightSource lightSources3[ NUM_LIGHTS ];
			     * };
			     *
			     * or
			     *
			     * @bind Material
			     * layout(std140) Material mymaterial = Material(
			     *  vec4(1.0, 0.8, 0.8, 1.0),
			     *  vec4(1.0, 0.8, 0.8, 1.0),
			     *  vec4(1.0, 0.8, 0.8, 1.0),
			     *  0.995
			     * );
			     */
				(
					-layout >> *eol
					>> omit [ ident ] >> *eol
					>> ident >> omit [ *eol >> (char_('{') | char_('=')) ]
					>> ignore
				)
			;

			ignore = !annot >> *(char_ - eol) >> eol;

			combi = annot >> declaration;
			start = *ignore >> combi % *ignore;

			// If debugging the parser, uncomment these lines - they are VERY useful for debugging
			/*
			   BOOST_SPIRIT_DEBUG_NODE(start);
			   BOOST_SPIRIT_DEBUG_NODE(combi);
			   BOOST_SPIRIT_DEBUG_NODE(ignore);
			   BOOST_SPIRIT_DEBUG_NODE(layout);
			   BOOST_SPIRIT_DEBUG_NODE(declaration);
			   BOOST_SPIRIT_DEBUG_NODE(annot);
			   BOOST_SPIRIT_DEBUG_NODE(ident);
			 */
		}

private:
		qi::rule<It, qi::blank_type> ignore;
		qi::rule<It, std::string(), qi::blank_type> layout;
		qi::rule<It, std::string(), qi::blank_type> ident, declaration, annot;
		qi::rule<It, std::pair<std::string, std::string>(), qi::blank_type> combi;
		qi::rule<It, Map(), qi::blank_type> start;
	};
	
template <typename It> struct locationGrammar : qi::grammar<It, Map(), qi::blank_type> {
public:
		locationGrammar() : locationGrammar::base_type(start)
		{
			using namespace qi;
			ident = lexeme [ alnum >> *alnum ];

			annot = "@location" >> +digit >> eol;

			layout = "layout(" >> *(omit [ ident ]) >> ')';

			declaration =

			    /*
			     * Match to things like:
			     * @bind texture0
			     * uniform sampler2D texture;
			     *
			     * or
			     *
			     * @bind texture0
			     * uniform
			     * sampler2D
			     * texture
			     * ;
			     */
				(
					-layout >> *eol
					>> omit [ +((ident | eol) >> (*eol >> !char_(';'))) ] >> *eol
					>> ident >> *eol
					>> ';' >> eol
				)

				// or
				|

			    /*
			     * Match to things like:
			     * @bind Light
			     * layout(std140) uniform LightSources {
			     *  LightSource lightSources[ NUM_LIGHTS ];
			     * };
			     *
			     * or
			     *
			     * @bind Light
			     * layout(std140)
			     * uniform
			     * LightSources
			     * {
			     *  LightSource lightSources[ NUM_LIGHTS ];
			     * };
			     *
			     * or
			     *
			     * @bind Light2
			     * uniform LightSources3
			     * {
			     *  LightSource lightSources3[ NUM_LIGHTS ];
			     * };
			     *
			     * or
			     *
			     * @bind Material
			     * layout(std140) Material mymaterial = Material(
			     *  vec4(1.0, 0.8, 0.8, 1.0),
			     *  vec4(1.0, 0.8, 0.8, 1.0),
			     *  vec4(1.0, 0.8, 0.8, 1.0),
			     *  0.995
			     * );
			     */
				(
					-layout >> *eol
					>> omit [ ident ] >> *eol
					>> ident >> omit [ *eol >> (char_('{') | char_('=')) ]
					>> ignore
				)
			;

			ignore = !annot >> *(char_ - eol) >> eol;

			combi = annot >> declaration;
			start = *ignore >> combi % *ignore;

			// If debugging the parser, uncomment these lines - they are VERY useful for debugging
			/*
			   BOOST_SPIRIT_DEBUG_NODE(start);
			   BOOST_SPIRIT_DEBUG_NODE(combi);
			   BOOST_SPIRIT_DEBUG_NODE(ignore);
			   BOOST_SPIRIT_DEBUG_NODE(layout);
			   BOOST_SPIRIT_DEBUG_NODE(declaration);
			   BOOST_SPIRIT_DEBUG_NODE(annot);
			   BOOST_SPIRIT_DEBUG_NODE(ident);
			 */
		}

private:
		qi::rule<It, qi::blank_type> ignore;
		qi::rule<It, std::string(), qi::blank_type> layout;
		qi::rule<It, std::string(), qi::blank_type> ident, declaration, annot;
		qi::rule<It, std::pair<std::string, std::string>(), qi::blank_type> combi;
		qi::rule<It, Map(), qi::blank_type> start;
	};
};

}
}
#endif /* GLRPARSER_H */
