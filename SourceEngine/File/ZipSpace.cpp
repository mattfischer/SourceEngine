#include "File/ZipSpace.hpp"

namespace File {

#pragma pack(push, 1)
struct FileHeader {
	int signature;
	short version;
	short flags;
	short compression;
	short modTime;
	short modDate;
	int crc;
	int sizeCompressed;
	int sizeUncompressed;
	short nameLength;
	short extraLength;
};

struct DirectoryHeader {
	int signature;
	short version;
	short minVersion;
	short flags;
	short compression;
	short modTime;
	short modDate;
	int crc;
	int sizeCompressed;
	int sizeUncompressed;
	short nameLength;
	short extraLength;
	short commentLength;
	short disk;
	short attrInternal;
	int attrExternal;
	int offset;
};

#define EOD_SIG 0x06054b50
struct EndOfDirectoryHeader {
	int signature;
	short disk;
	short dirDisk;
	short numDirRecordsDisk;
	short numDirRecords;
	int dirSize;
	int dirOffset;
	short commentSize;
};
#pragma pack(pop)

class ZipFile : public File {
public:
	ZipFile(const FileHeader *header)
	{
		setSize(header->sizeUncompressed);
		mData = (unsigned char*)header + sizeof(FileHeader) + header->nameLength + header->extraLength;
	}

	virtual int doRead(void *buffer, int size)
	{
		memcpy(buffer, mData + pointer(), size);
		
		return size;
	}

private:
	const unsigned char *mData;
};

ZipSpace::ZipSpace(const unsigned char *data, size_t size)
{
	EndOfDirectoryHeader *eodHeader;
	size_t offset;

	for(offset = size - sizeof(EndOfDirectoryHeader); offset >= 0; offset--) {	
		eodHeader = (EndOfDirectoryHeader*)(data + offset);
		if(eodHeader->signature == EOD_SIG) {
			break;
		}
	}

	offset = eodHeader->dirOffset;
	for(int i=0; i<eodHeader->numDirRecords; i++) {
		DirectoryHeader *dirHeader = (DirectoryHeader*)(data + offset);
		offset += sizeof(DirectoryHeader);
		std::string name((char*)(data + offset), dirHeader->nameLength);
		mOffsetMap[name] = dirHeader->offset;

		offset += dirHeader->nameLength + dirHeader->extraLength + dirHeader->commentLength;
	}

	mData = data;
}

File *ZipSpace::open(const std::string &name)
{
	ZipFile *file = 0;

	if(exists(name)) {
		file = new ZipFile((FileHeader*)(mData + mOffsetMap[name]));
	}

	return file;
}

bool ZipSpace::exists(const std::string &name)
{
	return mOffsetMap.find(name) != mOffsetMap.end();
}

}