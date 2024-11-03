#ifndef __SYREN_XML_NODE_H__
#define __SYREN_XML_NODE_H__

#include "pugixml/src/pugixml.hpp"
#include <Windows.h>
#include <DirectXMath.h>
#include <sstream>
#include <format>
#include <string>

#define VALUE_METHOD_DECLARATION(type) type value(type def = type()) const;
#define REF_VALUE_METHOD_DECLARATION(type) type value(const type& def = type()) const;

#define SETVALUE_METHOD_DECLARATION(type) void setValue(type value);
#define REF_SETVALUE_METHOD_DECLARATION(type) void setValue(const type& value);

class ChildSentinel {};
class ChildRange;
class XMLNode
{
	using base_node_type = pugi::xml_node;
	friend class ChildIterator;
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

	[[nodiscard]] ChildRange childs() const;

	[[nodiscard]] XMLNode child(const std::string& identifier) const
	{
		return pBaseNode_.child(identifier.c_str());
	}
	/// @brief возвращает дочерний нод с определнным идентификатором. если такого нода нет - то создает.
	/// @param identifier 
	/// @return 
	[[nodiscard]] XMLNode saveGetChild(const std::string& identifier)
	{
		auto child = pBaseNode_.child(identifier.c_str());
		if (!child) {
			child = pBaseNode_.append_child(identifier.c_str());
		}

		return XMLNode(child);
	}
	//<primitive types>
	VALUE_METHOD_DECLARATION(bool);
	VALUE_METHOD_DECLARATION(int);
	VALUE_METHOD_DECLARATION(unsigned int);
	VALUE_METHOD_DECLARATION(size_t);
	VALUE_METHOD_DECLARATION(float);
	VALUE_METHOD_DECLARATION(double);

	SETVALUE_METHOD_DECLARATION(bool);
	SETVALUE_METHOD_DECLARATION(int);
	SETVALUE_METHOD_DECLARATION(unsigned int);
	SETVALUE_METHOD_DECLARATION(size_t);
	SETVALUE_METHOD_DECLARATION(float);
	SETVALUE_METHOD_DECLARATION(double);
	//</primitive types>

	//<reference types>
	REF_VALUE_METHOD_DECLARATION(std::string);
	REF_VALUE_METHOD_DECLARATION(std::wstring);
	REF_VALUE_METHOD_DECLARATION(DirectX::XMFLOAT3);
	REF_VALUE_METHOD_DECLARATION(DirectX::XMFLOAT2);
	REF_VALUE_METHOD_DECLARATION(DirectX::XMVECTOR);
	REF_VALUE_METHOD_DECLARATION(DirectX::XMMATRIX);
	REF_VALUE_METHOD_DECLARATION(DirectX::XMFLOAT4X4);

	REF_SETVALUE_METHOD_DECLARATION(std::string);
	REF_SETVALUE_METHOD_DECLARATION(std::wstring);
	REF_SETVALUE_METHOD_DECLARATION(DirectX::XMFLOAT3);
	REF_SETVALUE_METHOD_DECLARATION(DirectX::XMFLOAT2);
	REF_SETVALUE_METHOD_DECLARATION(DirectX::XMVECTOR);
	REF_SETVALUE_METHOD_DECLARATION(DirectX::XMMATRIX);
	REF_SETVALUE_METHOD_DECLARATION(DirectX::XMFLOAT4X4);

	//</reference types>

	operator bool() const
	{
		return pBaseNode_;
	}

	template<typename T>
	T value(const T& def = T()) const
	{
		return value(def);
	}
private:

	auto text() const
	{
		return const_cast<base_node_type&>(pBaseNode_).text();
	}

	base_node_type pBaseNode_;
};

class ChildIterator
{
	using base_node_type = XMLNode::base_node_type;
public:
	ChildIterator(base_node_type pNode)
		noexcept(std::is_nothrow_copy_constructible_v<base_node_type>)
		:
		xmlNode_(pNode.first_child())
	{}


	ChildIterator& operator++()
	{
		pBaseNode() = pBaseNode().next_sibling();
		return *this;
	}
	ChildIterator operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	friend bool operator==(const ChildIterator& a, const ChildIterator& b) noexcept;

	friend bool operator==(const ChildIterator& a, const ChildSentinel&) noexcept;

	friend bool operator==(const ChildSentinel&, const ChildIterator& a) noexcept;

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
		return const_cast<base_node_type&>(xmlNode_.pBaseNode_);
	}

	XMLNode xmlNode_;
};

class ChildRange
{
public:
	ChildRange(ChildIterator begin) : begin_(begin) {};

	auto begin()
	{
		return begin_;
	}

	constexpr auto end()
	{
		return ChildSentinel{};
	}


private:
	ChildIterator begin_;
};


#undef VALUE_METHOD_DECLARATION
#undef REF_VALUE_METHOD_DECLARATION

#undef SETVALUE_METHOD_DECLARATION
#undef REF_SETVALUE_METHOD_DECLARATION

#endif //!__SYREN_XML_NODE_H__
