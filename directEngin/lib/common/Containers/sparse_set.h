#pragma once

#include <vector>
#include <array>

template<typename Entity,unsigned N>
class SparseSet
{
public:


	using key_type = Entity;
	using densed_container_type = std::vector<key_type>;
	using sparse_container_type = std::vector<key_type>;

	static constexpr key_type tombstone = N-1;


	class reverse_iterator
	{
	public:

		reverse_iterator(densed_container_type::iterator it) :_curIt(it) {}

		auto operator++(int) const noexcept
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		auto operator++() noexcept
		{
			--_curIt;
			return *this;
		}

		bool operator==(const reverse_iterator& other) const noexcept
		{
			return _curIt == other._curIt;
		}

		bool operator!=(const reverse_iterator& other) const noexcept
		{
			return _curIt != other._curIt;
		}

		auto& operator*() const noexcept
		{
			return *_curIt;
		}

	private:
		densed_container_type::iterator _curIt;
	};

	SparseSet() : _sparse(N, tombstone)
	{
		_densed.reserve(N);
		_densed.push_back(tombstone);
		//reserve(N);
		//_data.reserve(capacity);
	}

	auto begin() noexcept
	{
		// using reverse_iterator to enshure that modification of SparseArray
		// during iteration will not trigger any exeptions
		return _densed.begin();//reverse_iterator(densed.begin() + _data.size()); 
	}

	auto end() noexcept
	{
		return _densed.end()-1;
	}

	bool contains(key_type key) const
	{
		return _sparse[key] != tombstone;
	}


	auto operator[](key_type key) const
	{
		return _sparse[key];
	}

	void add(key_type key)
	{
		if (contains(key))
			return;

		auto pos = size();
		_densed[pos] = key;
		_densed.push_back(tombstone);//always keep tombstone last. it cannot throw exeption cause we
		_sparse[key] = pos;
	}

	bool remove(key_type key)
	{
		const auto pos = _sparse[key];
		if (pos == tombstone)// Data not exist
			return false;
		_densed.pop_back();//remove tombstone
		const auto last = _densed.back();//get real Data
		_sparse[last] = pos;
		std::swap(_densed[pos],_densed.back());
		_sparse[key] = tombstone;//nullifie key
		_densed.back() = tombstone;//set tombstone
		return true;
	}

	void clear() noexcept
	{
		_sparse.clear();
		_densed.clear();
	}

	auto size() const noexcept
	{
		return _densed.size() - 1;
	}

	virtual ~SparseSet() {};
		 
private:

	sparse_container_type _sparse;
	densed_container_type _densed;

};