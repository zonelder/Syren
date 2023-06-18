#include "PointedException.h"
#include <sstream>


PointedException::PointedException(int line,const char*file) noexcept
	:
	_line(line),
	_file(file)
{}

const char* PointedException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< getOriginalString();
	p_buffer = oss.str();
	return p_buffer.c_str();
}

const char* PointedException::getType() const noexcept {
	return "Default Exception";
}

const std::string& PointedException::getFile() const noexcept {
	return _file;
}

int PointedException::getLine() const noexcept{

	return _line;
}

std::string PointedException::getOriginalString()const noexcept {
	std::ostringstream oss;
	oss << "[FILE] " << _file << std::endl
		<< "[LINE] " << _line;
	return oss.str();
}
