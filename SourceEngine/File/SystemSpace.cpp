#include "File/SystemSpace.hpp"

#include "File/File.hpp"

#include <fstream>

namespace File {

class SystemFile : public File {
public:
	SystemFile(const std::string &filename)
		: mFile(filename.c_str(), std::ios_base::in | std::ios_base::binary)
	{
		mFile.seekg(0, std::ios_base::end);
		mSize = mFile.tellg();
		mFile.seekg(0);
	}

	virtual ~SystemFile() {}

	virtual void read(void *buffer, int size)
	{
		mFile.read((char*)buffer, size);
	}

	virtual void seek(int pos)
	{
		mFile.seekg(pos);
	}

	virtual int size()
	{
		return mSize;
	}

private:
	std::ifstream mFile;
	int mSize;
};

SystemSpace::SystemSpace(const std::string &directory)
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

File *SystemSpace::open(const std::string &name)
{
	File *file = 0;

	if(exists(name)) {
		std::string filename = getFilename(mDirectory, name);
		file = new SystemFile(filename);
	}

	return file;
}

bool SystemSpace::exists(const std::string &name)
{
	std::string filename = getFilename(mDirectory, name);
	std::ifstream file(filename.c_str());
	return !file.fail();
}

}