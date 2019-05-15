#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <io.h>
#include "osplatformutil.h"

#if defined I_OS_WIN32
inline void getFiles(std::string path, std::vector<std::string>& files)
{
	long   hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
inline std::vector<std::string> listDir(const std::string& dir)
{
	std::vector<std::string> files;
	getFiles(dir, files);

	return files;
}
inline std::string path_join(std::string parent_dir, std::string sub_dir)
{
	return parent_dir + "\\" + sub_dir;
}
#else
std::vector<std::string> listDir(const std::string& dir)
{
	//TODO:linux
}
#endif

template<class T>
inline void str2any(const std::string &string_temp, T &int_temp)
{

	std::stringstream stream(string_temp);
	stream >> int_temp;
}

inline std::vector<std::string> split(char* s)
{
	std::vector<std::string> result;
	char *p;
	const char *delim = " ";
	p = strtok(s, delim);
	while (p) {
		result.push_back(std::string(p));
		p = strtok(NULL, delim);
	}

	return result;
}



