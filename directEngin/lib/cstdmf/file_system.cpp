//#include "pch.h"

#include "file_system.h"

#include <filesystem>

namespace fileSystem
{
	/// @brief get extension of file
	/// @param file name. or full file name
	/// @return 
	std::string getExtension(const std::string& file)
	{
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastDot != std::string::npos)
		{
			return file.substr(lastDot + 1);
		}
		return {};
	}

	std::wstring getExtension(const std::wstring& file)
	{
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastDot != std::wstring::npos)
		{
			return file.substr(lastDot + 1);
		}
		return {};
	}

	/// @brief get local file name without extension
	/// @param file some file name.
	/// @return 
	std::string getFilename(const std::string& file)
	{
		size_t lastSlash = file.find_last_of("/\\");
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastSlash == std::string::npos)
		{
			return file;
		}

		return file.substr(lastSlash + 1, lastDot);
	}

	bool fileExist(const std::string& file)
	{
		return std::filesystem::exists(file);
	}

	/// @brief get fullpath from relative path
	/// @param path #relative or fullpath to object;
	/// @return 
	std::string resolvePath(const std::string& path)
	{
		if (path.empty())
			return "";

		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto absolutePath = std::filesystem::absolute(fsPath, ec);

		if (ec)
		{
			//TODO(log) error to resolve path
			return "";
		}

		return absolutePath.string();
	}

	std::string relativePath(const std::string& path)
	{
		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto relativePath = std::filesystem::relative(fsPath, std::filesystem::current_path(), ec);
		if (ec)
		{
			//TODO(log) error to resolve path
			return "";
		}
		return relativePath.string();
	}

	std::wstring relativePath(const std::wstring& path)
	{
		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto relativePath = std::filesystem::relative(fsPath, std::filesystem::current_path(), ec);
		if (ec)
		{
			//TODO(log) error to resolve path
			return L"";
		}
		return relativePath.wstring();
	}

	std::wstring resolvePath(const std::wstring& path)
	{
		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto absolutePath = std::filesystem::absolute(fsPath, ec);
		if (ec)
		{
			//TODO(log) error to resolve path
			return L"";
		}
		return absolutePath.wstring();
	}

	std::string simplifyPath(const std::string& path)
	{
		auto path1 = fileSystem::resolvePath(path);
		return fileSystem::relativePath(path1);
	}

	std::wstring simplifyPath(const std::wstring& path)
	{
		auto path1 = fileSystem::resolvePath(path);
		return fileSystem::relativePath(path1);
	}


}


