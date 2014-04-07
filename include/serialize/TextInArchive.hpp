#ifndef TEXTINARCHIVE_H_
#define TEXTINARCHIVE_H_

#include <string>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>

namespace glr
{
namespace serialize
{

/**
 * 
 */
class TextInArchive : public boost::archive::text_iarchive
{
public:
	TextInArchive(std::ifstream& ifstream);
	virtual ~TextInArchive();
};

}
}

#endif /* TEXTINARCHIVE_H_ */
