#ifndef __SYREN_SPARSE_SET_H__
#define __SYREN_SPARSE_SET_H__

#include <iterator>


namespace detail
{

	template<class Cont,typename T>
	class iterator_base
	{
	public:
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		explicit iterator_base(value_type* ptr = nullptr) noexcept : _ptr(ptr) {};

		bool operator==(const iterator_base& other) const noexcept = default;
		auto operator<=>(const iterator_base& other) const noexcept = default;

		iterator_base& operator++() noexcept { ++_ptr; return *this; }
		iterator_base operator++(int) noexcept { auto copy = *this; ++(*this); return copy; }
		reference operator*() const noexcept { return *_ptr; }
		pointer operator->() const noexcept { return _ptr; }

		iterator_base operator+(difference_type n) const noexcept { return iterator_base(_ptr + n); }
		difference_type operator-(iterator_base other) const noexcept { return _ptr - other._ptr; }

	private:
		value_type* _ptr;
	};

	constexpr size_t cache_line_size = 64u;
	template<typename T>
	T* aligned_alloc(size_t count)
	{
		void* ptr = ::operator new(count * sizeof(T), std::align_val_t{ cache_line_size });
		return static_cast<T*>(ptr);
	}

	template<typename T>
	void aligned_free(T* ptr)  noexcept
	{
		::operator delete(ptr, std::align_val_t{ cache_line_size });
	}

	template<typename Entity>
	constexpr size_t elements_per_cache_line = cache_line_size / sizeof(Entity);
	
	template<typename Entity>
	size_t optimal_capacity(size_t desired) noexcept
	{
		return ((desired + elements_per_cache_line<Entity> - 1) / elements_per_cache_line<Entity>)* elements_per_cache_line<Entity>;
	}
}

template<typename Entity>
class SparseSet
{
	using This = SparseSet<Entity>;
	using value_type = Entity;
	static_assert(std::is_unsigned_v<value_type>, "Key must be unsigned");
	static_assert(sizeof(Entity) <= sizeof(size_t), "Entity size insufficient for addressing.");
	static constexpr value_type tombstone = std::numeric_limits<Entity>::max();
public:

	using iterator = detail::iterator_base<This,Entity>;
	using const_iterator = detail::iterator_base<This,const Entity>;

	SparseSet(size_t initial_capacity =  4096) : 
		_sparse(nullptr),
		_dense(nullptr),
		_capacity(0u),
		_size(0u)
	{
		resize(detail::optimal_capacity<Entity>(initial_capacity));
	}

	~SparseSet()
	{
		detail::aligned_free(_sparse);
		detail::aligned_free(_dense);
	}

	//disable copy as its not needed yet.
	SparseSet(const SparseSet&) = delete;
	SparseSet& operator=(const SparseSet&) = delete;

	SparseSet(SparseSet&& other) noexcept
		: _sparse(other._sparse),
		_dense(other._dense),
		_capacity(other._capacity),
		_size(other._size)
	{
		other._sparse = nullptr;
		other._dense = nullptr;
		other._capacity = 0;
		other._size = 0;
	}
	SparseSet& operator=(SparseSet&& other) noexcept {
		if (this != &other) 
		{
			detail::aligned_free(_sparse);
			detail::aligned_free(_dense);
			_sparse = other._sparse;
			_dense = other._dense;
			_capacity = other._capacity;
			_size = other._size;
			other._sparse = nullptr;
			other._dense = nullptr;
			other._capacity = 0;
			other._size = 0;
		}
		return *this;
	}



	void resize(size_t new_capacity)
	{
		Entity* new_sparse = detail::aligned_alloc<Entity>(new_capacity);
		Entity* new_dense = detail::aligned_alloc<Entity>(new_capacity);

		if (_sparse)
		{
			std::copy(_sparse, _sparse + std::min(_capacity,new_capacity), new_sparse);
			std::copy(_dense, _dense + std::min(_size, new_capacity), new_dense);
		}
		else
		{
			std::fill_n(new_sparse, new_capacity, tombstone);
		}

		detail::aligned_free(_sparse);
		detail::aligned_free(_dense);

		_sparse = new_sparse;
		_dense = new_dense;
		_capacity = new_capacity;
	}


	bool contains(Entity e) const noexcept
	{
		return e < _capacity && _sparse[e] < _size && _dense[_sparse[e]] == e;
	}

	void add(Entity e)
	{
		if (contains(e)) return;
		if(e > _capacity) resize(detail::optimal_capacity<Entity>(e + 1));
		if (_size >= _capacity) resize(detail::optimal_capacity< Entity >(_capacity * 2u));

		_sparse[e] = static_cast<Entity>(_size);
		_dense[_size++] = e;
	}

	bool remove(Entity e) noexcept
	{
		if (!contains(e)) return false;

		const Entity pos = _sparse[e];
		const Entity last = _dense[_size - 1];

		_dense[pos] = last;
		_sparse[last] = pos;
		//std::swap(_dense[pos], _sparse[last]);
		_sparse[e] = tombstone;
		--_size;

		return true;
	}

	iterator begin() noexcept { return iterator(_dense); }
	iterator end() noexcept { return iterator(_dense + _size);}

	const_iterator begin() const noexcept { return const_iterator(_dense); }
	const_iterator end() const noexcept { return const_iterator(_dense + _size); };

	const_iterator::pointer data() const noexcept { return _dense; }
	size_t size() const noexcept { return _size; }
	size_t capacity()  const noexcept { return _capacity; }
	Entity index_of(Entity e) const noexcept { return e < _capacity ? _sparse[e] : tombstone; }
private:

	Entity* _sparse;
	Entity* _dense;
	size_t _capacity;
	size_t _size;
};

#endif