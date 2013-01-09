/*
 * File.h
 *
 *  Created on: 2010-12-23
 *      Author: jarrett
 */

#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

namespace oglre {

namespace io {

class File {
public:
	File(char* file, bool read = true, bool write = false, bool truncate = true, bool append = false);
	virtual ~File();

	void close();
	bool isOpen();
	static bool exists(char* file);

	bool write(char* text);
	bool writeln(char* text);
	void flush();

	void skipLine();

	char* read(int i = 1);
	char* readln(int i = 256);

	int readInt();
	char* readToken(int max = 256);
	
	static std::string getFileContents(const std::string filename);

private:
	fstream* stream;

	void open(char* file, bool read, bool write, bool truncate, bool append);

	void skipChars(char c);

	bool read_, write_;

};

}

}

#endif /* FILE_H_ */
