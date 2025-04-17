#pragma once
#include "sparse_set.h"

template<typename Data,typename Entity,unsigned N>
class SparseArray
{
public:
	using key_type = Entity;
	using sparse_set_type = SparseSet<Entity, N>;

/*
	class reverse_iterator
	{
	public:

		reverse_iterator(data_densed_container_type::iterator it) :_curIt(it) {}

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
	*/
	SparseArray() :_begin(new Data[N]), _end(_begin)
	{
	}

	~SparseArray()
	{
		delete[] _begin;
	}

	auto ebegin() noexcept
	{
		return _set.begin();
	}

	auto eend() noexcept
	{
		return _set.end();
	}

	auto ebegin() const noexcept { return _set.begin(); };

	auto eend() const noexcept { return _set.end(); };
	/*
	* 	auto begin() noexcept
	{
		// using reverse_iterator to enshure that modification of SparseArray
		// during iteration will not trigger any exeptions
		return reverse_iterator(_data.begin() + _data.size()); 
	}
	auto end() noexcept
	{
		return reverse_iterator(_data.begin());
	*/

	bool contains(key_type key) const
	{
		return _set.contains(key);
	}

	Data& operator[](key_type key)
	{
		return *(_begin +_set[key]);
	}

	const Data& operator[](key_type key) const
	{
		return *(_begin + _set[key]);
	}

	Data& add(key_type key)
	{
		if(_set.contains(key))
			return this->operator[](key);

		_set.add(key);
		return emplace_back();
	}


	bool remove(key_type key)
	{
		if (!_set.contains(key))
			return false;
		auto pos = _set[key];
		_set.remove(key);
		std::swap(*(_begin + pos),*(_end - 1));
		pop_back();
		return true;

	}

	auto begin() noexcept
	{
		return _begin;//_data.begin();
	}
	auto end() noexcept
	{
		return _end;//_data.end();
	}

	auto begin() const noexcept
	{
		return _begin;//_data.begin();
	}

	auto end() const noexcept
	{
		return _end;//_data.end();
	}


	auto size() const noexcept
	{
		return _set.size();
	}

	auto empty() const noexcept
	{
		return _set.size() == 0;
	}
		 
private:

	auto& emplace_back() noexcept(std::is_nothrow_constructible_v<Data>)
	{
		Data* pData = new (_end++) Data{};
		return *pData;
	}

	auto pop_back() noexcept(std::is_nothrow_destructible_v<Data>)
	{
		--_end;
		_end->~Data();
	}

	sparse_set_type _set;
	Data* _begin;
	Data* _end;
};