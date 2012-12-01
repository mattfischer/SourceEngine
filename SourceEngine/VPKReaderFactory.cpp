#include "VPKReaderFactory.hpp"

#include "VPKDirectory.hpp"
#include "IReader.hpp"

#include <fstream>
#include <algorithm>

class VPKReader : public IReader {
public:
	VPKReader(VPKDirectory::FileInfo &fileInfo, VPKDirectory &directory)
		: mFileInfo(fileInfo),
		  mDirectory(directory)
	{
		mPointer = 0;
	}

	virtual void read(char *buffer, int size)
	{
		int bytesRead = 0;
		if(mPointer < mFileInfo.preloadBytes) {
			int readSize = std::min((unsigned int)size, mFileInfo.preloadBytes - mPointer);
			memcpy(buffer, mFileInfo.preloadData + mPointer, readSize);
			mPointer += readSize;
			bytesRead = readSize;
		}

		if(!mFile.good()) {
			std::string filename = mDirectory.getArchiveName(mFileInfo.archiveIndex, mDataStart);
			mFile.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
		}
		mFile.seekg(mDataStart + mFileInfo.entryOffset + mPointer - mFileInfo.preloadBytes);
		mFile.read(buffer + bytesRead, size - bytesRead);
		mPointer += size;
	}

	virtual void seek(int pos)
	{
		mPointer = pos;
	}

	virtual int size()
	{
		return mFileInfo.preloadBytes + mFileInfo.entryLength;
	}

private:
	VPKDirectory::FileInfo &mFileInfo;
	VPKDirectory &mDirectory;
	unsigned int mPointer;
	std::ifstream mFile;
	int mDataStart;
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
		reader = new VPKReader(info, mDirectory);
	}

	return reader;
}

bool VPKReaderFactory::exists(const std::string &name)
{
	return mDirectory.exists(name);
}
