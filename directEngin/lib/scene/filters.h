#ifndef __FILTERS_H__
#define __FILTERS_H__
#include <tuple>
#include "scene/component_manager.h"
#include "scene/Entity.h"

namespace filters
{


	struct filter_type {};
	template<class... Args>
	struct With
	{
		using ComponentFilterWith = filter_type;
	};



	template<class... Args>
	struct Without
	{
		using t = With<Args...>;
		using ComponentFilterWithout = filter_type;
	};


	template<class TWith, class TWithout>
	concept is_component_filters = requires
	{
		typename TWith::ComponentFilterWith;
		typename TWithout::ComponentFilterWithout;
	};

	template<class T>
	concept is_with_filter = requires{
		typename T::ComponentFilterWith;
	};

	template<class T>
	concept is_without_filter = requires{
		typename T::ComponentFilterWithout;
	};

	template<class T>
	concept is_component_filter = is_with_filter<T> || is_without_filter<T>;

	template<class ...Args>
	concept has_component_filters = (is_component_filter<Args> ||...);

	template<class... Args>
	concept has_not_filters = !has_component_filters<Args...>;

	template<typename Pools, typename Entity>
	inline bool all_of(const Pools& pools, const Entity& entt) noexcept
	{
		return std::apply([entt](auto*... pool) { return (pool->contains(entt) && ...); }, pools);
	}

	template<typename Pools, typename Entity>
	inline bool none_of(const Pools& pools, const Entity& entt) noexcept
	{
		return std::apply([entt](auto*... pool) { return !(pool->contains(entt) || ...); }, pools);
	}

	template<class Wt, class Wtout>
	class ComponentView;

	template<class... WithArgs, class... WithoutArgs>
	class ComponentView<With<WithArgs...>, Without<WithoutArgs...>>
	{
	public:

		using with_tuple = std::tuple< ComponentPool<WithArgs>*...>;
		using without_tuple = std::tuple<ComponentPool< WithoutArgs>*...>;
		using entity_iterator = SparseSet<EntityID>::iterator;

		ComponentView(ComponentManager& scene) :
			_includes({ scene.getPool<WithArgs>()... }),
			_excludes({ scene.getPool<WithoutArgs>()... })
		{
			initialize_iterators();
		}

		class iterator
		{
			using entity_iterator_type = entity_iterator;
		public:
			iterator(with_tuple& include, without_tuple& exclude, entity_iterator_type it, entity_iterator_type end) :
				_include(include),
				_exclude(exclude),
				_current(it),
				_end(end)
			{
				validate_current();
			}

			auto current() const noexcept { return _current; }

			iterator operator++(int) const noexcept
			{
				auto copy = *this;
				++(*this);

				return copy;
			}

			iterator& operator++() noexcept
			{
				++_current;
				validate_current();
				return *this;
			}

			bool operator==(const iterator& other) const noexcept
			{
				return _current == other._current;
			}

			auto operator*() noexcept
			{
				return get_components(*_current);
			}


		private:
			bool is_valid() const noexcept
			{
				return all_of(_include, *_current) && none_of(_exclude, *_current);
			}
			void validate_current()
			{
				while (_current != _end && !is_valid()) { ++_current; };
			}

			auto get_components(EntityID entt) const noexcept
			{
				return std::tuple<EntityID, WithArgs&...>(
					entt,
					(std::get<ComponentPool<WithArgs>*>(_include)->operator[](entt))...
				);
			}

			with_tuple& _include;
			without_tuple& _exclude;
			entity_iterator_type _current;
			entity_iterator_type _end;
		};


		template<class T>
		T& get(const Entity& entt) noexcept
		{
			return get<T>(entt.getID());
		}

		template<class T>
		auto& get(const EntityID& entt) noexcept
		{
			static_assert (isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->operator[](entt);
		}


		bool contains(const EntityID& entt) const noexcept
		{
			return all_of(_includes, entt) && none_of(_excludes, entt);
		}

		template<class T>
		bool has(const EntityID& entt) noexcept
		{
			static_assert (isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->contains(entt);
		}

		template<class T>
		static constexpr bool isWith = (... || std::is_same_v<T, WithArgs>);

		iterator begin() noexcept
		{
			return iterator(_includes, _excludes, _begin, _end);
		}

		auto end() noexcept
		{
			return iterator(_includes, _excludes, _end, _end);
		}




	private:
		void initialize_iterators() 
		{
			// Find smallest pool to optimize iteration
			auto* smallest = std::get<0>(_includes);
			size_t min_size = std::numeric_limits<size_t>::max();
			std::apply([&](auto*... pools) {
				((pools->size() < min_size ? (min_size = pools->size(), _begin = pools->index_begin(),_end = pools->index_end(), 0) : 0), ...);
				}, _includes);
		}

		with_tuple _includes;
		without_tuple _excludes;
		entity_iterator _begin;
		entity_iterator _end;
	};
}

#endif