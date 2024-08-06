#pragma once

#include <vector>

template<typename Data,typename Entity,unsigned N>
class SparseArray
{
public:


	using key_type = Entity;
	using densed_container_type = std::vector<key_type>;
	using sparse_container_type = std::vector<key_type>;
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
		return _data[_sparse[key]];
	}

	Data& add(const key_type& key) noexcept
	{
		if (contains(key))
			return this->operator[key];

		auto pos = _densed.size();
		_densed.push_back(key);
		_sparse[key] = pos;
		_data.push_back();
		return _data.back();
	}

	void remove(const key_type& key) noexcept
	{
		auto& pos = _sparse[key];
		if (pos == tombstone)// Data not exist
			return;

		const auto last = _densed.back();
		_sparse[last] = pos;
		pos = tombstone;
		std::swap(_densed[pos],_densed.back());
		_densed.pop_back();

		std::swap(_data[pos], _data.back());
		_data.pop_back();

	}

		 
private:

	sparse_container_type _sparse;
	densed_container_type _densed;
	data_densed_container_type _data;

};