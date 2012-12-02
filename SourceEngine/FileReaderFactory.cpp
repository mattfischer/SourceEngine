#include "FileReaderFactory.hpp"

#include "IReader.hpp"

#include <fstream>

class FileReader : public IReader {
public:
	FileReader(const std::string &filename)
		: mFile(filename.c_str(), std::ios_base::in | std::ios_base::binary)
	{
		mFile.seekg(0, std::ios_base::end);
		mSize = mFile.tellg();
		mFile.seekg(0);
	}

	virtual void FileReader::read(char *buffer, int size)
	{
		mFile.read(buffer, size);
	}

	virtual void FileReader::seek(int pos)
	{
		mFile.seekg(pos);
	}

	virtual int FileReader::size()
	{
		return mSize;
	}

private:
	std::ifstream mFile;
	int mSize;
};

FileReaderFactory::FileReaderFactory(const std::string &directory)
: mDirectory(directory)
{
}

std::string getFilename(const std::string &directory, const std::string &name)
{
	std::string filename;
	if(directory == "") {
		filename = name;
	} else {
		filename = directory + "\\" + name;
	}

	return filename;
}

IReader *FileReaderFactory::open(const std::string &name)
{
	IReader *reader = 0;

	if(exists(name)) {
		std::string filename = getFilename(mDirectory, name);
		reader = new FileReader(filename);
	}

	return reader;
}

bool FileReaderFactory::exists(const std::string &name)
{
	std::string filename = getFilename(mDirectory, name);
	std::ifstream file(filename.c_str());
	return !file.fail();
}