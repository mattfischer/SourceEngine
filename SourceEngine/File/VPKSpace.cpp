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
		setSize(mFileInfo.preloadBytes + mFileInfo.entryLength);
	}

	virtual ~VPKFile() {}

	virtual int doRead(void *buffer, int size)
	{
		int bytesRead = 0;
		unsigned int ptr = pointer();

		if(ptr < mFileInfo.preloadBytes) {
			int readSize = std::min((unsigned int)size, mFileInfo.preloadBytes - ptr);
			memcpy(buffer, mFileInfo.preloadData + ptr, readSize);
			ptr += readSize;
			bytesRead = readSize;
		}

		if(!mFile.is_open()) {
			std::string filename = mDirectory.getArchiveName(mFileInfo.archiveIndex, mDataStart);
			mFile.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
		}
		mFile.seekg(mDataStart + mFileInfo.entryOffset + ptr - mFileInfo.preloadBytes);
		mFile.read((char*)buffer + bytesRead, size - bytesRead);

		return size;
	}

private:
	Format::VPK::FileInfo &mFileInfo;
	Format::VPK &mDirectory;
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