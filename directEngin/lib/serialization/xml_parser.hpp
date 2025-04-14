#pragma once
#include "resmgr/xml_node.h"
#include <string>
#include <iostream>

class XMLParser
{
public:

	bool loadString(const std::string& xmlStr)
	{
		return handleResult(doc_.load_string(xmlStr.c_str()));
	}
	bool loadFile(const std::string& file)
	{
		return handleResult(doc_.load_file(file.c_str()));
	}

	XMLNode root() const
	{
		return XMLNode(doc_.root().first_child());
	}

private:
	
	static bool handleResult(const pugi::xml_parse_result& res)
	{
		if (!res)
		{
			std::cerr << "Parsing Error:: " << res.description() << std::endl;
			return false;
		}
		return true;
	}

	pugi::xml_document doc_;
};
