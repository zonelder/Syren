#ifndef __SYREN_SPARSE_PAGE_H__
#define __SYREN_SPARSE_PAGE_H__
#include "sparse_set.h"
#include <vector>
#include <memory>
#include <iterator>




template<typename Data,typename Entity>
class SparsePage
{
	using This = SparsePage<Data, Entity>;
public:
	using iterator = detail::iterator_base<This, Data>;
	using const_iterator = detail::iterator_base<This, const Data>;
	friend iterator;
	friend const_iterator;
	static_assert(std::is_unsigned_v<Entity>, "Entity must be unsigned integer");
	class alignas(64) Page
	{
		friend iterator;
		friend const_iterator;
	public:
		static constexpr size_t PAGE_SIZE = 4096;

		Page()
		{
			_data = static_cast<Data*>(::operator new(sizeof(Data) * PAGE_SIZE));
		}

		~Page()
		{
			for (size_t i = 0; i < _count; ++i) _data[i].~Data();
			::operator delete(_data);
		}

		Data& get(Entity local_id)
		{
			return _data[_set.index_of(local_id)];
		}

		const Data& get(Entity local_id) const
		{
			return _data[_set.index_of(local_id)];
		}

		bool contains(Entity local_id) const noexcept { return _set.contains(local_id); };

		Data& add(Entity local_id)
		{
			if (_set.contains(local_id)) return get(local_id);
			new(&_data[_count]) Data();
			_set.add(local_id);
			return _data[_count++];
		}

		bool remove(Entity local_id) noexcept
		{
			if (!_set.contains(local_id)) return false;
			const size_t idx = _set.index_of(local_id);
			_set.remove(local_id);
			auto last = _data + (_count - 1);
			std::swap(_data[idx], *last);
			last->~Data();
			--_count;
			return true;
		}

	private:
		SparseSet<Entity> _set;
		Data* _data{ nullptr };
		size_t _count{ 0u };
	};


public:

	using page_container = std::vector<std::unique_ptr<Page>>;
	using page_iterator = page_container::iterator;
	using const_page_iterator = page_container::const_iterator;
	static constexpr size_t PAGE_SIZE = Page::PAGE_SIZE;
	SparsePage()
	{
		_pages.reserve(16);
	}
	/*TODO rebuild to exeption free imp
	Data& operator[](Entity id)
	{
		auto [page_idx, local_id] = decompose(id);
		return get_page(page_idx)->get(local_id);
	}
	*/
	Data& add(Entity id) noexcept
	{
		auto [page_idx, local_id] = decompose(id);
		return get_or_create_page(page_idx)->add(local_id);
	}

	bool remove(Entity id) noexcept
	{
		auto [page_idx, local_id] = decompose(id);
		auto page = get_save_page(page_idx);
		return page && page->remove(local_id);
	}

	bool page_exist(Entity id) const noexcept
	{
		auto [page_idx, local_id] = decompose(id);
		return get_save_page(page_idx);
	}

	bool contains(Entity id) const noexcept
	{
		auto [page_idx, local_id] = decompose(id);
		auto page = get_save_page(page_idx);
		return page && page->contains(local_id);
	}

	/*
	* @brief unsave get data.Use it only when you can guarantee that data exist.
	* 
	*/
	Data& get(Entity id) noexcept
	{
		auto [page_idx, local_id] = decompose(id);
		auto page = get_page(page_idx);
		return page->get(id);
	}

	const Data& get(Entity id) const noexcept
	{
		auto [page_idx, local_id] = decompose(id);
		auto page = get_page(page_idx);
		return page->get(id);
	}

	iterator begin() noexcept
	{
		return iterator(_pages.begin(), _pages.end());
	}

	const_iterator begin() const noexcept
	{
		return const_iterator(_pages.begin(), _pages.end());
	}

	iterator end() noexcept
	{
		return iterator(_pages.end(),_pages.end());
	}

	const_iterator end() const noexcept
	{
		return const_iterator(_pages.end(),_pages.end());
	}

private:

	inline std::pair<size_t, Entity> decompose(Entity id) const noexcept
	{
		const size_t page_idx = id / Page::PAGE_SIZE;
		const size_t local_id = id % Page::PAGE_SIZE;
		return { page_idx, local_id };
	}

	//ensure that page exist before using this methods
	inline Page* get_page(size_t page_idx) const noexcept
	{
		return _pages[page_idx].get();
	}

	//this methods do a check of existence and return nullptr if page is not exist
	inline Page* get_save_page(size_t page_idx) const noexcept
	{
		if (_pages.size() > page_idx)
			return _pages[page_idx].get();

		return nullptr;
	}

	inline Page* get_or_create_page(size_t page_idx) noexcept
	{
		if (page_idx >= _pages.size())
			_pages.resize(page_idx + 1);

		if(!_pages[page_idx])
			_pages[page_idx] = std::make_unique<Page>();

		return _pages[page_idx].get();
	}
	//TODO make deque with bitmask ant test its perfomance.
	page_container _pages;
};



namespace detail
{
	template<class HandleData,class Entity,class Data>
	class iterator_base<SparsePage<HandleData,Entity>,Data>
	{
		using Cont = SparsePage<HandleData, Entity>;
		using Page = Cont::Page;
		using page_iterator = std::conditional_t<std::is_const_v<Data>,typename Cont::const_page_iterator,typename Cont::page_iterator>;
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Data;
		using difference_type = std::ptrdiff_t;
		using pointer = Data*;
		using reference = Data&;
		iterator_base(page_iterator begin, page_iterator end) noexcept :
			_current(find_first_non_empty(begin, end)),
			_end(end),
			_comp_idx(0u)
		{
		}

		reference operator*() noexcept
		{
			return (*_current)->_data[_comp_idx];
		}

		pointer operator->() const noexcept
		{
			return ((*_current)->_data + _comp_idx);
		}

		iterator_base& operator++() noexcept
		{
			if (++_comp_idx < (*_current)->_count) return *this;
			_current = find_first_non_empty(++_current, _end);
			_comp_idx = 0;
			return *this;
		}

		iterator_base operator++(int) const noexcept
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		bool operator==(const iterator_base& other) const noexcept
		{
			return _current == other._current && _comp_idx == other._comp_idx;
		}

	private:

		static page_iterator find_first_non_empty(page_iterator start, page_iterator end)
		{
			while (start != end && (!*start || (*start)->_count == 0)) { ++start; };
			return start;
		}

		page_iterator _current;
		page_iterator _end;
		size_t _comp_idx;
	};
}
#endif
