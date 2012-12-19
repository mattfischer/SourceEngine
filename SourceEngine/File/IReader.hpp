#ifndef FILE_IREADER_HPP
#define FILE_IREADER_HPP

namespace File {

class IReader {
public:
	virtual ~IReader() {}

	virtual void read(void *buffer, int size) = 0;
	virtual void seek(int pos) = 0;
	virtual int size() = 0;
};

}

#endif