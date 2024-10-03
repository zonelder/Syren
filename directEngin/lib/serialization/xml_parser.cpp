#include "xml_parser.hpp"
#include <DirectXMath.h>
#include <sstream>

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
	return { x, y, z};
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


childIterator XMLNode::begin() noexcept
{
	return childIterator(pBaseNode_);
}

[[nodiscard]]
bool operator==(const childIterator& a, const childIterator& b) noexcept
{
	return a.pBaseNode() == b.pBaseNode();
}

[[nodiscard]]
bool operator==(const childIterator& a,const ChildSentinel&) noexcept
{
	return a.pBaseNode() == nullptr;
}

[[nodiscard]]
bool operator==(const ChildSentinel& ,const childIterator& a) noexcept
{
	return a.pBaseNode() == nullptr;
}