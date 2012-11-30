#include "FileReaderFactory.hpp"

#include "IReader.hpp"

#include <fstream>

class FileReader : public IReader {
public:
	FileReader(const std::string &filename)
		: mFile(filename.c_str(), std::ios_base::in | std::ios_base::binary)
	{
	}

	virtual void FileReader::read(char *buffer, int size)
	{
		mFile.read(buffer, size);
	}

	virtual void FileReader::seek(int pos)
	{
		mFile.seekg(pos);
	}

private:
	std::ifstream mFile;
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
	if(exists(name)) {
		std::string filename = getFilename(mDirectory, name);
		return new FileReader(filename);
	} else {
		return 0;
	}
}

bool FileReaderFactory::exists(const std::string &name)
{
	std::string filename = getFilename(mDirectory, name);
	std::ifstream file(filename.c_str());
	return !file.fail();
}