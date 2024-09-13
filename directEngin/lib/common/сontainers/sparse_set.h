#pragma once

#include <vector>
#include <array>

template<typename Entity,unsigned N>
class SparseSet
{
	using key_type = Entity;
	static constexpr key_type tombstone = N - 1;
public:
	class sentinel{};

	class iterator
	{
	public:
		iterator(Entity* ptr) noexcept : _ptr(ptr) {};

		bool operator==(const iterator& other) const noexcept
		{
			return _ptr == other._ptr;
		}

		bool operator==(const sentinel& other) const noexcept
		{
			return *_ptr == tombstone;
		}

		iterator& operator++() noexcept
		{
			++_ptr;
			return *this;
		}

		iterator operator++(int) noexcept
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		Entity& operator*() const noexcept
		{
			return *_ptr;
		}

		Entity& operator->() const noexcept
		{
			return *_ptr;
		}


	private:
		Entity* _ptr;
	};




	/*
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
	*/


	SparseSet() : _pSparse( new Entity[N]),_densedBegin(new Entity[N]),_densedEnd(_densedBegin),_size(0)
	{
		for (size_t i = 0;i < N;++i)
		{
			_pSparse[i] = tombstone;
			_densedBegin[i] = tombstone;
		}
	}

	auto begin() noexcept
	{
		// using reverse_iterator to enshure that modification of SparseArray
		// during iteration will not trigger any exeptions
		return iterator(_densedBegin);
	}

	auto end() noexcept
	{
		return sentinel{};
	}

	bool contains(key_type key) const
	{
		return _pSparse[key] != tombstone;
	}


	auto operator[](key_type key) const
	{
		return _pSparse[key];
	}

	void add(key_type key)
	{
		if (contains(key))
			return;

		auto pos = size();
		*_densedEnd = key;
		++_densedEnd;
		_pSparse[key] = pos;
		++_size;
	}

	bool remove(key_type key)
	{
		const auto pos = _pSparse[key];
		if (pos == tombstone)// Data not exist
			return false;
		_pSparse[*_densedEnd] = pos;
		std::swap(_densedBegin[pos], *_densedEnd);
		_pSparse[key] = tombstone;
		*_densedEnd = tombstone;
		--_densedEnd;
		--_size;
		return true;
	}

	auto size() const noexcept
	{
		return _size;
	}

	virtual ~SparseSet() 
	{
		delete[] _pSparse;
		delete[] _densedBegin;
		_densedEnd = nullptr;
	};
		 
private:

	Entity* _pSparse;

	Entity* _densedBegin;

	//end look at back element
	Entity* _densedEnd;
	size_t _size;

};