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
	using index_iterator = detail::index_iterator<This,Entity>;
	using const_index_iterator = detail::index_iterator<This,const Entity>;
	using pair_iterator = detail::pair_iterator<This, Data>;
	using const_pair_iterator = detail::pair_iterator<This, const Data>;
	static_assert(std::is_unsigned_v<Entity>, "Entity must be unsigned integer");
	class alignas(64) Page
	{
		friend iterator;
		friend const_iterator;
		friend index_iterator;
		friend const_index_iterator;
		friend pair_iterator;
		friend const_pair_iterator;

	public:
		using entity_iterator = SparseSet<Entity>::iterator;
		static constexpr size_t PAGE_SIZE = 4096;

		Page(size_t page_index)
			: _page_idx(page_index)
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

		Entity get_entt(size_t index) const noexcept
		{
			return _set.entity_at(index) + static_cast<Entity>(_page_idx * PAGE_SIZE);
		}

	private:

		SparseSet<Entity> _set;
		Data* _data{ nullptr };
		size_t _page_idx;
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

	iterator				begin() noexcept { return iterator(_pages.begin(), _pages.end()); }
	iterator				end() noexcept { return iterator(_pages.end(),_pages.end()); }

	const_iterator			begin() const noexcept { return const_iterator(_pages.begin(), _pages.end()); }
	const_iterator			end() const noexcept { return const_iterator(_pages.end(),_pages.end()); }

	index_iterator			index_begin() noexcept { return index_iterator(_pages.begin(), _pages.end()); }
	index_iterator			index_end() noexcept { return index_iterator(_pages.end(), _pages.end()); }

	const_index_iterator	index_begin() const noexcept { return const_index_iterator(_pages.begin(), _pages.end()); }
	const_index_iterator	index_end() const noexcept { return const_index_iterator(_pages.end(), _pages.end()); }

	pair_iterator			pair_begin()	noexcept { return pair_iterator(_pages.begin(), _pages.end()); }
	pair_iterator			pair_end()		noexcept { return pair_iterator(_pages.end(), _pages.end()); }

	const_pair_iterator		pair_begin() const noexcept { return const_pair_iterator(_pages.begin(), _pages.end()); }
	const_pair_iterator		pair_end()   const noexcept { return const_pair_iterator(_pages.end(), _pages.end()); }

private:

	void shrink_to_fit() noexcept
	{
		while (!_pages.empty())
		{
			auto& p = _pages.back();
			if (!p || p->_count == 0) _pages.pop_back();
		}
		//_pages.shrink_to_fit();
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
			_pages[page_idx] = std::make_unique<Page>(page_idx);

		return _pages[page_idx].get();
	}
	//TODO make deque with bitmask ant test its perfomance.
	page_container _pages;
	std::vector<size_t> _page_table;
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

	template<class HandleData, class Entity,class IndexType>
	class index_iterator<SparsePage<HandleData, Entity>, IndexType>
	{
	public:
		using Cont = SparsePage<HandleData, Entity>;
		using Page = typename Cont::Page;
		using page_iterator = typename Cont::page_iterator;
		using const_page_iterator = typename Cont::const_page_iterator;
		using current_page_iterator = std::conditional_t<std::is_const_v<IndexType>, const_page_iterator, page_iterator>;

		using iterator_category = std::forward_iterator_tag;
		using value_type = IndexType;
		using difference_type = std::ptrdiff_t;
		using pointer = IndexType*;
		using reference = IndexType;

		index_iterator(current_page_iterator begin, current_page_iterator end) noexcept
			: _current(find_first_non_empty(begin, end)), _end(end), _comp_idx(0) {
		}

		reference operator*() const noexcept 
		{
			return (*_current)->get_entt(_comp_idx);
		}

		index_iterator& operator++() noexcept 
		{
			if (++_comp_idx < (*_current)->_count) return *this;
			_current = find_first_non_empty(++_current, _end);
			_comp_idx = 0;
			return *this;
		}

		index_iterator operator++(int) noexcept 
		{
			auto tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const index_iterator& other) const noexcept 
		{
			return _current == other._current && _comp_idx == other._comp_idx;
		}

	private:
		static current_page_iterator find_first_non_empty(current_page_iterator start, current_page_iterator end) noexcept 
		{
			while (start != end && (!*start || (*start)->_count == 0)) ++start;
			return start;
		}

		current_page_iterator _current;
		current_page_iterator _end;
		size_t _comp_idx;
	};

	template<class HandleData, class Entity, class Data>
	class pair_iterator< SparsePage<HandleData, Entity>, Data>
	{
	public:
		using Cont = SparsePage<HandleData, Entity>;
		using Page = typename Cont::Page;
		using page_iterator = typename Cont::page_iterator;
		using const_page_iterator = typename Cont::const_page_iterator;
		using current_page_iterator = std::conditional_t<std::is_const_v<Data>, const_page_iterator, page_iterator>;

		using iterator_category = std::forward_iterator_tag;
		using value_type = std::pair<Entity, Data&>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type;

		pair_iterator(current_page_iterator begin, current_page_iterator end) noexcept
			: _current(find_first_non_empty(begin, end)), _end(end), _comp_idx(0) {
		}

		reference operator*() const noexcept 
		{
			auto& page = **_current;
			return { page.get_entt(_comp_idx), page._data[_comp_idx]};
		}

		pair_iterator& operator++() noexcept 
		{
			if (++_comp_idx < (*_current)->_count) return *this;
			_current = find_first_non_empty(++_current, _end);
			_comp_idx = 0;
			return *this;
		}

		pair_iterator operator++(int) noexcept 
		{
			auto tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const pair_iterator& other) const noexcept 
		{
			return _current == other._current && _comp_idx == other._comp_idx;
		}

	private:
		static current_page_iterator find_first_non_empty(current_page_iterator start, current_page_iterator end) noexcept 
		{
			while (start != end && (!*start || (*start)->_count == 0)) ++start;
			return start;
		}

		current_page_iterator _current;
		current_page_iterator _end;
		size_t _comp_idx;
	};
}
#endif
