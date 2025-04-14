#ifndef __FAMILY_GENERATOR_H__
#define __FAMILY_GENERATOR_H__
#include "scene_base.h"

class Family
{
    static const auto identifier()
    {
        static ComponentID v = 0;
        return v++;
    }
public:
	template<typename T>
    static ComponentID type_id() noexcept
    {
        static const ComponentID id = identifier();
        return id;
    }
};

#endif