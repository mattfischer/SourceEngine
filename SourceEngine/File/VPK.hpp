#ifndef FILE_VPK_HPP
#define FILE_VPK_HPP

#include <map>
#include <string>

namespace File {

class VPK {
public:
	VPK(const std::string &filename);

	struct FileInfo {
		unsigned int archiveIndex;
		unsigned int entryOffset;
		unsigned int entryLength;
		unsigned int preloadBytes;
		char *preloadData;
	};

	bool exists(const std::string &name);
	FileInfo &lookup(const std::string &name);

	std::string getArchiveName(unsigned short index, int &startOffset);

private:
	typedef std::map<std::string, FileInfo> FileMap;
	typedef std::map<std::string, FileMap> PathMap;
	typedef std::map<std::string, PathMap> ExtMap;
	ExtMap mDirectory;
	std::string mFilename;
	int mDataStart;
};

}

#endif