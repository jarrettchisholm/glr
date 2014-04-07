#ifndef TEXTOUTARCHIVE_H_
#define TEXTOUTARCHIVE_H_

#include <string>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>

namespace glr
{
namespace serialize
{

/**
 * 
 */
class TextOutArchive : public boost::archive::text_oarchive
{
public:
	TextOutArchive(std::ofstream& ofstream);
	virtual ~TextOutArchive();
};

}
}

#endif /* TEXTOUTARCHIVE_H_ */
