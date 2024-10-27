#ifndef __XML_NODE_INL_H__
#define __XML_NODE_INL_H__
#include "xml_node.h"
#include <DirectXMath.h>
#include <sstream>
#include <format>

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
void XMLNode::setValue<std::string>(const std::string& def)
{
	text().set(def.c_str());
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


#endif 