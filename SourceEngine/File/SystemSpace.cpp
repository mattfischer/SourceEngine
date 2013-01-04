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
		int size = mFile.tellg();
		mFile.seekg(0);

		setSize(size);
	}

	virtual ~SystemFile() {}

	virtual int doRead(void *buffer, int size)
	{
		mFile.read((char*)buffer, size);
		return size;
	}

	virtual void doSeek(int pos)
	{
		mFile.seekg(pos);
	}

private:
	std::ifstream mFile;
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