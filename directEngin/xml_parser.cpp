#include "xml_parser.hpp"

template<>
std::string XMLNode::value<std::string>() const
{
	return value();
}

template<>
int XMLNode::value<int>() const
{
	return std::stoi(value());
}

template<>
float XMLNode::value<float>() const
{
	return std::stof(value());
}

template<>
bool XMLNode::value<bool>() const
{
	return std::stoi(value()) != 0;
}

template<>
size_t XMLNode::value<size_t>() const
{
	return std::stoul(value());
}

childIterator XMLNode::begin() const noexcept
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