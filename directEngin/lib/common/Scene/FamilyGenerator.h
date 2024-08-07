#pragma once
#include "SceneBase.h"
#include <type_traits>


namespace{
    template<unsigned N>
    struct reader 
    {
        friend auto counted_flag(reader<N>);
    };


    template<unsigned N>
    struct setter 
    {
        friend auto counted_flag(reader<N>) {}

        static constexpr unsigned n = N;
    };


    template<
        auto Tag,
        unsigned NextVal = 0>
    [[nodiscard]]
    consteval auto counter_impl() 
    {
        constexpr bool counted_past_value = requires(reader<NextVal> r) {
            counted_flag(r);
        };

        if constexpr (counted_past_value) 
        {
            return counter_impl<Tag, NextVal + 1>();           
        }
        else 
        {
            setter<NextVal> s;
            return s.n;
        }
    }


template< auto Tag = [] {},
          auto Val = counter_impl<Tag>()>
constexpr auto counter = Val;

}

// Дополнительное использование
//constexpr auto state1 = inject_state(reader<10>{});
//static_assert(reader<10>::value == 10);
//static_assert(state1.value == 11);



class Family
{
public:
	template<typename T>
	static constexpr ComponentID Type() noexcept
	{

		constexpr ComponentID value = counter<>;
		return value;
	}
};
