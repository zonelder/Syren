#pragma once
#include "../third_party/pugixml/src/pugixml.hpp"
#include <string>
#include <iostream>
class ChildSentinel {};

class XMLNode
{
	using base_node_type = pugi::xml_node;
	friend class childIterator;
public:


	XMLNode(base_node_type pNode) 
		noexcept(std::is_nothrow_copy_constructible_v<base_node_type>) 
		:
		pBaseNode_(pNode) 
	{}

	[[nodiscard]] std::string identifier() const
	{
		return pBaseNode_.name();
	}


	//TODO заменить на свою Обертку Range
	[[nodiscard]] auto childs(const std::string& identifier) const
	{
		return pBaseNode_.children(identifier.c_str());
	}

	[[nodiscard]] XMLNode child(const std::string& identifier) const
	{
		return pBaseNode_.child(identifier.c_str());
	}


	childIterator begin() const noexcept;

	constexpr auto end() const noexcept
	{
		return ChildSentinel{};
	}

	template<class T>
	T value() const;

	const char* value() const
	{
		return const_cast<base_node_type&>(pBaseNode_).text().get();
	}

private:
	base_node_type pBaseNode_;
};

class childIterator
{
	using base_node_type = XMLNode::base_node_type;
public:
	childIterator(base_node_type pNode) 
		noexcept(std::is_nothrow_copy_constructible_v<base_node_type>) 
		:
		xmlNode_(pNode.first_child()) 
	{}


	childIterator& operator++()
	{
		pBaseNode() = pBaseNode().next_sibling();
		return *this;
	}
	childIterator operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	friend bool operator==(const childIterator& a,const childIterator& b) noexcept;

	friend bool operator==(const childIterator& a,const ChildSentinel&) noexcept;

	friend bool operator==(const ChildSentinel&, const childIterator& a) noexcept;

	[[nodiscard]] XMLNode& operator*() const
	{
		return const_cast<XMLNode&>(xmlNode_);
	}

	[[nodiscard]] XMLNode* operator->() const
	{
		return const_cast<XMLNode*>(&xmlNode_);
	}

private:
	base_node_type& pBaseNode() const
	{
		return const_cast<base_node_type &>(xmlNode_.pBaseNode_);
	}

	XMLNode xmlNode_;
};


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
