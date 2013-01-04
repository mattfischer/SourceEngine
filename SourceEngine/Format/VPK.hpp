#ifndef FORMAT_VPK_HPP
#define FORMAT_VPK_HPP

#include <map>
#include <string>

namespace Format {

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
	struct FileMap {
		std::map<std::string, FileInfo> map;
	};

	struct PathMap {
		std::map<std::string, FileMap> map;
	};

	struct ExtMap {
		std::map<std::string, PathMap> map;
	};

	ExtMap mDirectory;
	std::string mFilename;
	int mDataStart;
};

}

#endif