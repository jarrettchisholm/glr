/*
 * File.cpp
 *
 *  Created on: 2010-12-23
 *      Author: jarrett
 */

#include "File.h"
#include <string.h>

#include <string>
#include <fstream>
#include <streambuf>

namespace oglre {

namespace io {

File::File(char* file, bool read, bool write, bool truncate, bool append) {
	open(file, read, write, truncate, append);
}

File::~File() {
	delete stream;
}

void File::open(char* file, bool read, bool write, bool truncate, bool append) {
	if (file == NULL || (read == false && write == false) || (truncate == true && append == true))
		return;

	read_ = read;
	write_ = write;

	_Ios_Openmode flags;
	if (!read) {
		flags = ios::out;
		if (truncate)
			flags |= ios::trunc;
		else if (append)
			flags |= ios::app;
	} else {
		flags = ios::in;
		if (write) {
			flags |= ios::out;
			if (truncate)
				flags |= ios::trunc;
			else if (append)
				flags |= ios::app;
		}
	}

	stream = new fstream(file, flags);

	// error check
	if (stream == NULL || !stream->is_open())
		cout << "Could not open file: " << file << endl;
}

bool File::isOpen() {
	if (stream == NULL)
		return false;

	return stream->is_open();
}

void File::close() {
	if (stream != NULL)
		stream->close();
}

bool File::exists(char* file) {
	if (file == NULL)
		return false;

	bool val = false;

	fstream temp(file, ios::in);
	if (temp.is_open()) {
		val = true;
	}
	temp.close();

	return val;
}

bool File::write(char* text) {
	if (text != NULL && isOpen() && write_) {
		*stream << text;
		return true;
	}

	return false;
}

bool File::writeln(char* text) {
	if (text != NULL && isOpen() && write_) {
		*stream << text << endl;
		return true;
	}

	return false;
}

void File::flush() {
	if (isOpen() && stream->good() && write_) {
		stream->flush();
	}
}

int File::readInt() {
	if (isOpen() && stream->good() && read_) {
		char* value = readToken();
		int retval = 0;
		if (isdigit(value[0]))
			retval = atoi(value);
		delete value;
		return retval;
	}

	return 0;
}

void File::skipChars(char c) {
	if (isOpen() && stream->good()) {
		char nextChar = stream->peek();
		while (nextChar == c) {
			stream->seekg(1, ios_base::cur);
			nextChar = stream->peek();

		}
	}
}

char* File::readToken(int max) {
	// skip any empty spaces and newlines
	skipChars(' ');
	//skipChars('\n');

	if (isOpen() && stream->good() && read_) {
		char* value = new char[max + 1];
		char c;
		char nextChar = stream->peek();
		int i = 0;
		while (nextChar != '\n' && nextChar != ' ' && i < max) {
			stream->get(c);
			value[i] = c;
			i++;
			nextChar = stream->peek();
		}

		//stream->getline(value, max, ' ');
		return value;
	}

	return NULL;
}

char* File::read(int i) {
	if (i >= 1 && isOpen() && stream->good() && read_) {
		char* value = new char[i];
		stream->read(value, i);
		return value;
	}

	return NULL;
}

// todo: test this method
void File::skipLine() {
	char* result = readln();

	while (result != NULL && strchr(result, '\n') != NULL) {
		delete result;
		result = readln();
	}

	if (result != NULL)
		delete result;
}

/**
 * File::readln(int i = 256)
 *	reads characters until either (i-1) characters are read or the eol or oef
 *	character is read
 */
char* File::readln(int i) {
	if (i >= 1 && isOpen() && stream->good() && read_) {
		char* value = new char[i + 1];
		stream->getline(value, i);
		return value;
	}

	return NULL;
}

std::string File::getFileContents(const std::string filename) {
	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());
	                 
	return str;
}

}

}
