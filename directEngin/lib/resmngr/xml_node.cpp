#include "xml_node.h"

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
	int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
	std::wstring wstr(size, L'\0'); // Создаем строку с нужным размером

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr.data(), size);
	return wstr;

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

DirectX::XMFLOAT3 XMLNode::value(const DirectX::XMFLOAT3& def) const
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

DirectX::XMFLOAT2 XMLNode::value(const DirectX::XMFLOAT2& def) const
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
	//TODO create string_utils.h and replace this mess with good-lokking code
	int size = WideCharToMultiByte(CP_UTF8, 0, val.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, val.c_str(), -1, str.data(), size, nullptr, nullptr);
	text().set(str.c_str());
}

void XMLNode::setValue(const DirectX::XMFLOAT3& def)
{
	text().set(std::format("{} {} {}", def.x, def.y, def.z).c_str());
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




///keep this at the end of file.
#undef DEFAULT_SETVALUE_METHOD_DEFINITION