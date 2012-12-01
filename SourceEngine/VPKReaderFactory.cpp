#include "VPKReaderFactory.hpp"

#include "IReader.hpp"

class VPKReader : public IReader {
public:
	VPKReader(VPKDirectory::FileInfo &fileInfo)
		: mFileInfo(fileInfo)
	{
	}

	virtual void read(char *buffer, int size)
	{
	}

	virtual void seek(int pos)
	{
	}

private:
	VPKDirectory::FileInfo &mFileInfo;
};

VPKReaderFactory::VPKReaderFactory(const std::string &filename)
: mDirectory(filename)
{
}

sp<IReader> VPKReaderFactory::open(const std::string &name)
{
	IReader *reader = 0;
	if(mDirectory.exists(name)) {
		VPKDirectory::FileInfo &info = mDirectory.lookup(name);
		reader = new VPKReader(info);
	}

	return reader;
}

bool VPKReaderFactory::exists(const std::string &name)
{
	return mDirectory.exists(name);
}
