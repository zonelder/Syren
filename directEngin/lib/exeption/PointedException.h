#pragma once
#include <exception>
#include <string>
class PointedException:public std::exception
{
public:
	PointedException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* getType() const noexcept;
	int getLine() const noexcept;
	const std::string& getFile() const noexcept;
	std::string getOriginalString() const noexcept;
private:
	int _line;
	std::string _file;
protected:
	mutable std::string p_buffer;
};

