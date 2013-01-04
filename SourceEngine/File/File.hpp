#ifndef FILE_FILE_HPP
#define FILE_FILE_HPP

namespace File {

class File {
public:
	File();
	virtual ~File() {}

	int read(void *buffer, int size);
	void seek(int pos);

	int size() { return mSize; }
	int pointer() { return mPointer; }

protected:
	void setSize(int size) { mSize = size; }

private:
	int mSize;
	int mPointer;

	virtual int doRead(void *buffer, int size) = 0;
	virtual void doSeek(int pos) {}
};

}

#endif