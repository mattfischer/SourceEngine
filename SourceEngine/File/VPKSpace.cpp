#include "File/VPKSpace.hpp"

#include "File/VPK.hpp"
#include "File/File.hpp"

#include <fstream>
#include <algorithm>

namespace File {

class VPKFile : public File {
public:
	VPKFile(::File::VPK::FileInfo &fileInfo, ::File::VPK &directory)
		: mFileInfo(fileInfo),
		  mDirectory(directory)
	{
		mPointer = 0;
	}

	virtual ~VPKFile() {}

	virtual void read(void *buffer, int size)
	{
		int bytesRead = 0;
		if(mPointer < mFileInfo.preloadBytes) {
			int readSize = std::min((unsigned int)size, mFileInfo.preloadBytes - mPointer);
			memcpy(buffer, mFileInfo.preloadData + mPointer, readSize);
			mPointer += readSize;
			bytesRead = readSize;
		}

		if(!mFile.is_open()) {
			std::string filename = mDirectory.getArchiveName(mFileInfo.archiveIndex, mDataStart);
			mFile.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
		}
		mFile.seekg(mDataStart + mFileInfo.entryOffset + mPointer - mFileInfo.preloadBytes);
		mFile.read((char*)buffer + bytesRead, size - bytesRead);
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
	::File::VPK::FileInfo &mFileInfo;
	::File::VPK &mDirectory;
	unsigned int mPointer;
	std::ifstream mFile;
	int mDataStart;
};

VPKSpace::VPKSpace(const std::string &filename)
: mDirectory(filename)
{
}

File *VPKSpace::open(const std::string &name)
{
	File *file = 0;
	if(mDirectory.exists(name)) {
		::File::VPK::FileInfo &info = mDirectory.lookup(name);
		file = new VPKFile(info, mDirectory);
	}

	return file;
}

bool VPKSpace::exists(const std::string &name)
{
	return mDirectory.exists(name);
}

}