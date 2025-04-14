#include "xml_node.h"
#include "cstdmf/string_converter.h"

#define DEFAULT_SETVALUE_METHOD_DEFINITION(type) \
void XMLNode::setValue(type v) \
{ \
	text().set(v); \
}


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

int XMLNode::value(int def) const
{
	return text().as_int(def);
}

std::string XMLNode::value(const std::string& def) const
{
	return text().as_string(def.c_str());
}

std::wstring XMLNode::value(const std::wstring& def) const
{
	//TODO create string_utils.h and replace this mess with good-lokking code
	auto str = text().as_string(nullptr);
	if (!str)
	{
		return def;
	}

	return stringHelper::to_wstring(str);

}


float XMLNode::value(float def) const
{
	return text().as_float(def);
}


bool XMLNode::value(bool def) const
{
	return text().as_bool(def);
}

size_t XMLNode::value(size_t def) const
{
	return text().as_ullong(def);
}


/////// DirectX Data Types ///////

DirectX::XMFLOAT4 XMLNode::value(const DirectX::XMFLOAT4& def) const
{
    std::stringstream ss(text().get());
    DirectX::XMFLOAT4 result;
    if (!(ss >> result.x >> result.y >> result.z >> result.w))
    {
        return def;
    }
    return result;
}


Vector3 XMLNode::value(const Vector3& def) const
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

Quaternion XMLNode::value(const Quaternion& def) const
{
	std::stringstream ss(text().get());
	float x, y, z,w;
	ss >> x >> y >> z >> w;
	if (ss.fail())
	{
		return def;
	}
	return { x, y, z, w };
}


Color XMLNode::value(const Color& def) const
{
	std::stringstream ss(text().get());
	float x, y, z, w;
	ss >> x >> y >> z >> w;
	if (ss.fail())
	{
		return def;
	}
	return { x, y, z, w };
}




Vector2 XMLNode::value(const Vector2& def) const
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


DirectX::XMVECTOR XMLNode::value(const DirectX::XMVECTOR& def) const
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


DirectX::XMMATRIX XMLNode::value(const DirectX::XMMATRIX& def) const
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

DirectX::XMFLOAT4X4 XMLNode::value(const DirectX::XMFLOAT4X4& def) const
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





////////////SETTERS/////////////////////

DEFAULT_SETVALUE_METHOD_DEFINITION(bool);
DEFAULT_SETVALUE_METHOD_DEFINITION(int);
DEFAULT_SETVALUE_METHOD_DEFINITION(unsigned int);
DEFAULT_SETVALUE_METHOD_DEFINITION(size_t);
DEFAULT_SETVALUE_METHOD_DEFINITION(float);
DEFAULT_SETVALUE_METHOD_DEFINITION(double);


void XMLNode::setValue(const std::string& val)
{
	text().set(val.c_str());
}


void XMLNode::setValue(const std::wstring& val)
{
	std::string str = stringHelper::to_string(val);

	text().set(str.c_str());
}

void XMLNode::setValue(const Vector3& def)
{
	text().set(std::format("{} {} {}", def[0], def[1], def[2]).c_str());
}

void XMLNode::setValue(const Quaternion& val)
{
	text().set(std::format("{} {} {} {}", val[0], val[1], val[2], val[3]).c_str());
}

void XMLNode::setValue(const Color& val)
{
	text().set(std::format("{} {} {} {}", val[0], val[1], val[2], val[3]).c_str());
}

void XMLNode::setValue(const DirectX::XMFLOAT4& v)
{
	text().set(std::format("{} {} {} {}", v.x, v.y, v.z, v.w).c_str());
}

void XMLNode::setValue(const DirectX::XMVECTOR& def)
{

	text().set(std::format("{} {} {} {}",
		DirectX::XMVectorGetByIndex(def, 0),
		DirectX::XMVectorGetByIndex(def, 1),
		DirectX::XMVectorGetByIndex(def, 2),
		DirectX::XMVectorGetByIndex(def, 3)
	).c_str());
}


[[nodiscard]] ChildRange XMLNode::childs() const
{
	return ChildRange(pBaseNode_);
}

void XMLNode::childs(const std::string& identifier, std::vector<XMLNode>& childs, int ensureSize)
{
	childs.clear();

	int currCount = 0;

	for (auto child : pBaseNode_.children(identifier.c_str()))
	{
		++currCount;
		childs.emplace_back(child);
	}

	for (; currCount <= ensureSize; ++currCount)
	{
		childs.emplace_back(pBaseNode_.append_child(identifier.c_str()));
	}

}


///keep this at the end of file.
#undef DEFAULT_SETVALUE_METHOD_DEFINITION