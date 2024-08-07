#pragma once
#include "sparse_set.h"
#include <vector>

template<typename Data,typename Entity,unsigned N>
class SparseArray : private SparseSet<Entity,N>
{
public:


	using key_type = Entity;
	using sparse_set_type = SparseSet<Entity, N>;
	using data_densed_container_type = std::vector<Data>;

	static constexpr key_type tombstone = N-1;


	class reverse_iterator
	{
	public:

		reverse_iterator(data_densed_container_type::iterator it):_curIt(it)

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
		sparse_set_type::reverse_iterator _itEnttID;
		data_densed_container_type::iterator _curIt;
	};

	SparseArray() noexcept : _sparse(N, tombstone),_densed(N,tombstone)
	{
		//reserve(N);
		_data.reserve(capacity);
	}

	void reserve(size_t capacity) noexcept
	{
		_densed.reserve(capacity);
		_sparse.reserve(capacity);
		_data.reserve(capacity);
	}

	auto begin() noexcept
	{
		// using reverse_iterator to enshure that modification of SparseArray
		// during iteration will not trigger any exeptions
		return reverse_iterator(_data.begin() + _data.size()); 
	}

	auto end() noexcept
	{
		return reverse_iterator(_data.begin());
	}

	bool contains(const key_type& key) const noexcept
	{
		return _sparse[key] != tombstone;
	}


	Data& operator[](const key_type& key) const noexcept
	{
		return _data[_set[key]];
	}

	Data& add(const key_type& key) noexcept
	{
		if(_set.contains(key))
			return this->operator[key];

		_set.add(key);
		_data.push_back();
		return _data.back();
	}

	bool remove(const key_type& key) noexcept
	{

		if (!_set.remove(key))
			return false;

		std::swap(_data[pos], _data.back());
		_data.pop_back();
		return true;

	}

		 
private:

	sparse_set_type _set;
	data_densed_container_type _data;

};