#include "VPKDirectory.hpp"

#include <fstream>
#include <iostream>

struct Header_v1
{
	unsigned int signature;
	unsigned int version;
	unsigned int treeLength;
};

struct Header_v2
{
	unsigned int signature;
	unsigned int version;
	unsigned int treeLength;
 
	int unknown1;
	unsigned int footerLength;
	int unknown3;
	int unknown4;
};

#pragma pack(push, 1)
struct DirectoryEntry
{
	unsigned int crc;
	unsigned short preloadBytes;
	unsigned short archiveIndex;
	unsigned int entryOffset;
 	unsigned int entryLength;
 	unsigned short Terminator;
};
#pragma pack(pop)

std::string readString(std::ifstream &file)
{
	std::string ret;
	char c;

	while(true) {
		file.read(&c, 1);
		if(c == '\0') {
			break;
		}
		ret += c;
	}
	return ret;
}

std::string &uppercase(std::string &str)
{
	for(unsigned int i=0; i<str.size(); i++) {
		str[i] = toupper(str[i]);
	}
	return str;
}

VPKDirectory::VPKDirectory(const std::string &filename)
{
	std::ifstream file(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	std::ofstream list("fileList.txt");

	Header_v1 header_v1;

	int dataStart;
	file.read((char*)&header_v1, sizeof(header_v1));

	switch(header_v1.version) {
		case 1:
		{
			dataStart = sizeof(header_v1) + header_v1.treeLength;
			break;
		}

		case 2:
		{
			Header_v2 header_v2;
			file.seekg(0);
			file.read((char*)&header_v2, sizeof(header_v2));
			dataStart = sizeof(header_v2) + header_v2.treeLength;
			break;
		}
	}

	while(true) {
		std::string ext = uppercase(readString(file));
		if(ext == "") {
			break;
		}
		PathMap &pathMap = mDirectory[ext];

		while(1) {
			std::string path = uppercase(readString(file));
			if(path == "") {
				break;
			}
			FileMap &fileMap = pathMap[path];

			while(1) {
				std::string filename = uppercase(readString(file));
				if(filename == "") {
					break;
				}
				list << path << "/" << filename << "." << ext << std::endl;
				FileInfo &info = fileMap[filename];

				DirectoryEntry entry;
				file.read((char*)&entry, sizeof(entry));
				info.archiveIndex = entry.archiveIndex;
				info.entryLength = entry.entryLength;
				info.entryOffset = entry.entryOffset;
				info.preloadBytes = entry.preloadBytes;
				if(entry.preloadBytes > 0) {
					info.preloadData = new char[entry.preloadBytes];
					file.read(info.preloadData, entry.preloadBytes);
				} else {
					info.preloadData = 0;
				}
			}
		}
	}
}

void splitPath(const std::string &name, std::string &ext, std::string &path, std::string &filename)
{
	size_t extpos = name.find('.');
	size_t fnpos = name.rfind('/');

	if(fnpos == name.npos) {
		fnpos = 0;
	} else {
		fnpos++;
	}

	ext = uppercase(name.substr(extpos));
	path = uppercase(name.substr(0, fnpos));
	filename = uppercase(name.substr(fnpos, extpos - fnpos));
}

bool VPKDirectory::exists(const std::string &name)
{
	std::string ext, path, filename;
	splitPath(name, ext, path, filename);

	if(mDirectory.find(ext) == mDirectory.end()) {
		return false;
	}

	PathMap &pathMap = mDirectory[ext];
	if(pathMap.find(path) == pathMap.end()) {
		return false;
	}

	FileMap &fileMap = pathMap[path];
	if(fileMap.find(filename) == fileMap.end()) {
		return false;
	}

	return true;
}

VPKDirectory::FileInfo emptyInfo;
VPKDirectory::FileInfo &VPKDirectory::lookup(const std::string &name)
{
	if(exists(name)) {
		std::string ext, path, name;
		splitPath(name, ext, path, name);

		return mDirectory[ext][path][name];
	} else {
		return emptyInfo;
	}
}