#include "File/VPKSpace.hpp"

#include "File/File.hpp"

#include "Format/VPK.hpp"

#include <fstream>
#include <algorithm>

namespace File {

class VPKFile : public File {
public:
	VPKFile(Format::VPK::FileInfo &fileInfo, Format::VPK &directory)
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
	Format::VPK::FileInfo &mFileInfo;
	Format::VPK &mDirectory;
	unsigned int mPointer;
	std::ifstream mFile;
	int mDataStart;
};

VPKSpace::VPKSpace(const std::string &filename)
: mVpk(filename)
{
}

File *VPKSpace::open(const std::string &name)
{
	File *file = 0;
	if(mVpk.exists(name)) {
		Format::VPK::FileInfo &info = mVpk.lookup(name);
		file = new VPKFile(info, mVpk);
	}

	return file;
}

bool VPKSpace::exists(const std::string &name)
{
	return mVpk.exists(name);
}

}