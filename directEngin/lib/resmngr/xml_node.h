#ifndef __SYREN_XML_NODE_H__
#define __SYREN_XML_NODE_H__


#pragma message("compiling file xml_node.h as new.")
#include "pugixml/src/pugixml.hpp"
#include <Windows.h>
#include <DirectXMath.h>
#include <sstream>
#include <format>
#include <string>

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

	/*
	//TODO заменить на свою Обертку Range
	[[nodiscard]] auto childs(const std::string& identifier) const
	{
		return pBaseNode_.children(identifier.c_str());
	}
	*/
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

	template<class T>requires (!std::is_fundamental_v<T>)
		T value(const T& def = T()) const;

	template<class T> requires std::is_fundamental_v<T>
	T value(T def = T()) const;

	operator bool() const
	{
		return pBaseNode_;
	}

	template<typename T>
	void setValue(const T& v);


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


#ifndef __SYREN_XML_NODE_IPP__
#define __SYREN_XML_NODE_IPP__
#pragma message("compiling file xml_node.ipp as new.")
template<>
int XMLNode::value<int>(int def) const
{
	return text().as_int(def);
}

template<>
std::string XMLNode::value<std::string>(const std::string& def) const
{
	return text().as_string(def.c_str());
}
template<>
std::wstring XMLNode::value<std::wstring>(const std::wstring& def) const
{
	//TODO create string_utils.h and replace this mess with good-lokking code
	auto str = text().as_string(nullptr);
	if (!str)
	{
		return def;
	}
	int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
	std::wstring wstr(size, L'\0'); // Создаем строку с нужным размером

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr.data(), size);
	return wstr;

}

template<>
float XMLNode::value<float>(float def) const
{
	return text().as_float(def);
}

template<>
bool XMLNode::value<bool>(bool def) const
{
	return text().as_bool(def);
}

template<>
size_t XMLNode::value<size_t>(size_t def) const
{
	return text().as_ullong(def);
}


/////// DirectX Data Types ///////
template<>
DirectX::XMFLOAT3 XMLNode::value<DirectX::XMFLOAT3>(const DirectX::XMFLOAT3& def) const
{
	std::stringstream ss(text().get());
	float x, y, z;
	ss >> x >> y >> z;
	if (ss.fail())
	{
		return def;
	}
	return { x, y, z };
}

template<>
DirectX::XMFLOAT2 XMLNode::value<DirectX::XMFLOAT2>(const DirectX::XMFLOAT2& def) const
{
	std::stringstream ss(text().get());
	float x, y;
	ss >> x >> y;
	if (ss.fail())
	{
		return def;
	}
	return { x,y };
}

template<>
DirectX::XMVECTOR XMLNode::value<DirectX::XMVECTOR>(const DirectX::XMVECTOR& def) const
{
	std::stringstream ss(text().get());
	float x, y, z, w;
	ss >> x >> y >> z >> w;
	if (ss.fail())
	{
		return def;
	}
	return DirectX::XMVectorSet(x, y, z, w);
}

template<>
DirectX::XMMATRIX XMLNode::value<DirectX::XMMATRIX>(const DirectX::XMMATRIX& def) const
{
	std::stringstream ss(text().get());
	DirectX::XMFLOAT4X4 matrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			ss >> matrix.m[i][j];
		}
	}
	if (ss.fail())
	{
		return def;
	}
	return DirectX::XMLoadFloat4x4(&matrix);
}

template<>
DirectX::XMFLOAT4X4 XMLNode::value<DirectX::XMFLOAT4X4>(const DirectX::XMFLOAT4X4& def) const
{
	std::stringstream ss(text().get());
	DirectX::XMFLOAT4X4 matrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			ss >> matrix.m[i][j];
		}
	}
	if (ss.fail())
	{
		return def;
	}
	return matrix;
}


///TODO реализуй value методы для других классов из DirectXMath


[[nodiscard]]
bool operator==(const ChildIterator& a, const ChildIterator& b) noexcept
{
	return a.pBaseNode() == b.pBaseNode();
}

[[nodiscard]]
bool operator==(const ChildIterator& a, const ChildSentinel&) noexcept
{
	return a.pBaseNode() == nullptr;
}

[[nodiscard]]
bool operator==(const ChildSentinel&, const ChildIterator& a) noexcept
{
	return a.pBaseNode() == nullptr;
}


////////////SETTERS/////////////////////

template<typename T>
void XMLNode::setValue(const T& v)
{
	text().set(v);
}



template<>
void XMLNode::setValue<std::string>(const std::string& val)
{
	text().set(val.c_str());
}

template<>
void XMLNode::setValue<std::wstring>(const std::wstring& val)
{
	//TODO create string_utils.h and replace this mess with good-lokking code
	int size = WideCharToMultiByte(CP_UTF8, 0, val.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, val.c_str(), -1, str.data(), size, nullptr, nullptr);
	text().set(str.c_str());
}

template<>
void XMLNode::setValue<DirectX::XMFLOAT3>(const DirectX::XMFLOAT3& def)
{
	text().set(std::format("{} {} {}", def.x, def.y, def.z).c_str());
}


template<>
void XMLNode::setValue<DirectX::XMVECTOR>(const DirectX::XMVECTOR& def)
{

	text().set(std::format("{} {} {} {}",
		DirectX::XMVectorGetByIndex(def, 0),
		DirectX::XMVectorGetByIndex(def, 1),
		DirectX::XMVectorGetByIndex(def, 2),
		DirectX::XMVectorGetByIndex(def, 3)
	).c_str());
}

[[nodiscard]]
ChildRange XMLNode::childs() const
{
	return ChildRange(pBaseNode_.first_child());
}

#endif //!__SYREN_XML_NODE_IPP__
#endif //!__SYREN_XML_NODE_H__
