#include "VPKFile.hpp"

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

VPKFile::VPKFile(const std::string &filename)
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
		std::string ext = readString(file);
		if(ext == "") {
			break;
		}
		PathMap &pathMap = mDirectory[ext];

		while(1) {
			std::string path = readString(file);
			if(path == "") {
				break;
			}
			FileMap &fileMap = pathMap[path];

			while(1) {
				std::string name = readString(file);
				if(name == "") {
					break;
				}
				list << path << "/" << name << "." << ext << std::endl;
				FileInfo &info = fileMap[name];

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