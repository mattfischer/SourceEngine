#ifndef VPK_DIRECTORY_HPP
#define VPK_DIRECTORY_HPP

#include <map>
#include <string>

class VPKDirectory {
public:
	VPKDirectory(const std::string &filename);

	struct FileInfo {
		unsigned int archiveIndex;
		unsigned int entryOffset;
		unsigned int entryLength;
		unsigned int preloadBytes;
		char *preloadData;
	};

	bool exists(const std::string &name);
	FileInfo &lookup(const std::string &name);

private:
	typedef std::map<std::string, FileInfo> FileMap;
	typedef std::map<std::string, FileMap> PathMap;
	typedef std::map<std::string, PathMap> ExtMap;
	ExtMap mDirectory;
};

#endif