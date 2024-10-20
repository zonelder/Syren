#ifndef __XML_NODE_H__
#define __XML_NODE_H__
#include "pugixml/src/pugixml.hpp"
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


	childIterator begin() noexcept;

	constexpr auto end() noexcept
	{
		return ChildSentinel{};
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

	friend bool operator==(const childIterator& a, const childIterator& b) noexcept;

	friend bool operator==(const childIterator& a, const ChildSentinel&) noexcept;

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
		return const_cast<base_node_type&>(xmlNode_.pBaseNode_);
	}

	XMLNode xmlNode_;
};

#endif //!__XML_NODE_H__
