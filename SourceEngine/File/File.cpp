#include "File/File.hpp"

namespace File {

File::File()
{
	mPointer = 0;
	mSize = 0;
}

int File::read(void *buffer, int size)
{
	int bytesRead = doRead(buffer, size);
	mPointer += bytesRead;

	return bytesRead;
}

void File::seek(int pos)
{
	doSeek(pos);

	mPointer = pos;
}

}