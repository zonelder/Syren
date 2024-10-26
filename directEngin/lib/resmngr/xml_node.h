#ifndef __XML_NODE_H__
#define __XML_NODE_H__
#include "pugixml/src/pugixml.hpp"
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

#endif //!__XML_NODE_H__
