#ifndef __SYREN_FILE_SYSTEM__
#define __SYREN_FILE_SYSTEM__

#include <string>


namespace fileSystem
{
	std::string getExtension(const std::string& file);
	std::wstring getExtension(const std::wstring& file);

	std::string getFilename(const std::string& file);

	bool fileExist(const std::string& file);

	std::string resolvePath(const std::string& path);

	std::string relativePath(const std::string& path);

	std::wstring relativePath(const std::wstring& path);

	std::wstring resolvePath(const std::wstring& path);

	std::string simplifyPath(const std::string& path);

	std::wstring simplifyPath(const std::wstring& path);
}


#endif