#ifndef IREADER_HPP
#define IREADER_HPP

class IReader {
public:
	virtual void read(char *buffer, int size) = 0;
	virtual void seek(int pos) = 0;
	virtual int size() = 0;
};

#endif