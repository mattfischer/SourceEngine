#ifndef VPKFILE_HPP
#define VPKFILE_HPP

#include <string>
#include <fstream>
#include <map>

class VPKFile {
public:
	VPKFile(const std::string &filename);

private:
	struct FileInfo {
		unsigned int archiveIndex;
		unsigned int entryOffset;
		unsigned int entryLength;
		unsigned int preloadBytes;
		char *preloadData;
	};
	typedef std::map<std::string, FileInfo> FileMap;
	typedef std::map<std::string, FileMap> PathMap;
	typedef std::map<std::string, PathMap> ExtMap;
	ExtMap mDirectory;
};
#endif