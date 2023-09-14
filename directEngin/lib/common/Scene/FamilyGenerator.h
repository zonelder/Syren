#pragma once


using ComponentID = unsigned int;

class Family
{
	static ComponentID GetIdentifier()
	{
		static ComponentID id = 0;
		return ++id;
	}
public:
	template<typename>
	static  ComponentID Type() noexcept
	{
		static const ComponentID value = GetIdentifier();
		return value;
	}
};
