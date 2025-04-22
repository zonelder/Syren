#pragma once
#include "sparse_set.h"
#include <stdexcept>

template<typename Data,typename Entity,unsigned N>
class SparseArray
{
public:
	using key_type = Entity;
	using sparse_set_type = SparseSet<Entity>;

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
	SparseArray()
	{
		_data = static_cast<Data*>(::operator new(sizeof(Data) * N));
		_end = _data;
	}

	~SparseArray()
	{
		for (auto it = _data; it != _end; ++it)
		{
			it->~Data();
		}

		::operator delete(_data);
	}

	Data& operator[](key_type key)
	{
		if (key < 0 || key >= N)
		{
			throw std::out_of_range("SparseArray::operator[]: index out of range.");
		}
		return *(_data +_set.index_of(key));
	}

	const Data& operator[](key_type key) const
	{
		if (key < 0 || key >= N)
		{
			throw std::out_of_range("SparseArray::operator[]: index out of range.");
		}
		return *(_data + _set.index_of(key));
	}

	Data& add(Entity id)
	{
		if (_set.contains(id)) return get(id);
		if (size() >= N)
		{
			throw std::out_of_range("SparseArray::add: container overflow.");
		}
		_set.add(id);
		Data* ptr = new(_end++)Data();
		return *ptr;
	}


	bool remove(key_type id)
	{
		if (!_set.contains(id)) return false;
		size_t idx = _set.index_of(id);
		_set.remove(id);
		auto last = _end - 1;
		std::swap(_data[idx], *last);
		last->~Data();
		--_end;
		return true;
	}

	bool contains(Entity id) const noexcept { return _set.contains(id); }
	Data& get(Entity id) noexcept { return _data[_set.index_of(id)]; }
	const Data& get(Entity& id) const noexcept { return _data[_set.index_of(id)]; }

	Data* begin() noexcept { return _data; }
	Data* end() noexcept { return _end; }

	const Data* begin() const noexcept { return _data; }
	const Data* end() const noexcept { return _end; };

	size_t size() const noexcept { return _set.size(); }
	bool empty() const noexcept { return _set.size() == 0; }

	sparse_set_type::iterator index_begin()			noexcept { return _set.begin(); }
	sparse_set_type::iterator index_end()				noexcept { return _set.end(); }

	sparse_set_type::const_iterator index_begin()		const noexcept { return _set.begin(); }
	sparse_set_type::const_iterator index_end()		const noexcept { return _set.end(); }

private:
	sparse_set_type _set;
	Data* _data;
	Data* _end;
};