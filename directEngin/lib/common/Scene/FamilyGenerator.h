#pragma once


using ComponentID = unsigned int;

class Family
{
	static ComponentID GetIdentifier()
	{

		static ComponentID id = 0u;
		return id++;
	}
public:
	template<typename>
	static  ComponentID Type() noexcept
	{

		static ComponentID value = GetIdentifier();


		return value;
	}
};
