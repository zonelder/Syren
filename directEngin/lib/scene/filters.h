#ifndef __FILTERS_H__
#define __FILTERS_H__
#include <tuple>
#include "scene/component_manager.h"
#include "scene/Entity.h"

namespace filters
{
	template<class... Args> struct With{};
	template<class... Args> struct Without{};

	template<class T> struct is_filter : std::false_type{};
	template<class... Args> struct is_filter < With<Args...>> : std::true_type {};
	template<class... Args> struct is_filter < Without<Args...>> : std::true_type{};
	template<class T> inline constexpr bool is_filter_v = is_filter<T>::value;

	template<class T> struct is_with_filter : std::false_type{};
	template<class... Args> struct is_with_filter<With<Args...>> : std::true_type{};
	template<class T> inline constexpr bool is_with_filter_v = is_with_filter<T>::value;

	template<class T> struct is_without_filter : std::false_type {};
	template<class... Args> struct is_without_filter<Without<Args...>> : std::true_type {};
	template<class T> inline constexpr bool is_without_filter_v = is_without_filter<T>::value;

	template<class...Args> inline constexpr bool is_contain_filters = (is_filter_v<Args> || ...);
	template<class...Args> inline constexpr bool is_filter_free = !is_contain_filters<Args...>;


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

	/**
	 * @brief Iterable view that represent entities with specific types.
	 * @tparam WithArgs... - #types that reqiures to be in entity
	 * @tparam WithoutArgs... - #types that should not be in entity;
	 */
	template<class... WithArgs, class... WithoutArgs>
	class ComponentView<With<WithArgs...>, Without<WithoutArgs...>>
	{
		template<class T>
		static constexpr bool has_component = (... || std::is_same_v<T, WithArgs>);

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
					(std::get<ComponentPool<WithArgs>*>(_include)->get(entt))...
				);
			}

			with_tuple& _include;
			without_tuple& _exclude;
			entity_iterator_type _current;
			entity_iterator_type _end;
		};

		/**
		 * @brief Method for get access to components that was requires with view
		 *
		 * @tparam T - type of component we want to get
		 * @param entt -#id of entity we a looking for
		 * @return component of entity
		 */
		template<class T> T& get(const Entity entt) noexcept { return get<T>(entt.getID()); }
		/**
		 * @brief Method for get access to components that was requires with view
		 *
		 * @tparam T - type of component we want to get
		 * @param entt -#id of entity we a looking for
		 * @return component of entity
		 */
		template<class T> const T& get(const Entity entt) const noexcept { return get<T>(entt.getID());}

		/**
		 * @brief Method for get access to components that was requires with view
		 *
		 * @tparam T - type of component we want to get
		 * @param entt -#id of entity we a looking for
		 * @return component of entity
		 */
		template<class T>
		T& get(const EntityID& entt) noexcept
		{
			static_assert (has_component<T>,"Attempt to get component data, that dont belong to view.");
			return std::get< ComponentPool<T>*>(_includes)->get(entt);
		}

		/**
		 * @brief Method for get access to components that was requires with view
		 *
		 * @tparam T - type of component we want to get
		 * @param entt -#id of entity we a looking for
		 * @return component of entity
		 */
		template<class T>
		const T& get(const EntityID entt) const noexcept
		{
			static_assert (has_component<T>, "Attempt to get component data, that dont belong to view.");
			return std::get< ComponentPool<T>*>(_includes)->get(entt);
		}

		/**
		* @brief Check wether entity pass the requirement of ComponentView
		* @param entt -#id of entity
		* @return true if entity pass requirements, false otherwise
		*/
		bool contains(const EntityID& entt) const noexcept { return all_of(_includes, entt) && none_of(_excludes, entt); }

		/**
		* @brief Check wether entity has some component that view requires.
		* @tparam T - # type of component.
		* @param entt - #id of entity
		* @return true if entity has component of type T, false otherwise
		*/
		template<class T>
		bool has(const EntityID& entt) noexcept
		{
			static_assert (has_component<T>, "Attempt to get component data, that dont belong to view.");
			return std::get< ComponentPool<T>*>(_includes)->contains(entt);
		}

		iterator begin() noexcept { return iterator(_includes, _excludes, _begin, _end); }

		auto end() noexcept { return iterator(_includes, _excludes, _end, _end); }

	private:
		void initialize_iterators() 
		{
			size_t index = 0;
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