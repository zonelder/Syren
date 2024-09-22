#pragma once
#include <set>
#include <string>

template<class T>
struct serializer
{
	static std::set<size_t> s_fields;
	void serialize(const T& comp)
	{
		for (auto field : s_fields)
		{
			serializeField<T,field>(comp);
		}
	}

	void deserialize(const T& comp)
	{

	}

};


//#define SERIALIZABLE(type) template<> serializer<type>;

#define SERIALIZE_FIELD(type,field)										\
	constexpr size_t fieldID = std::hash<std::string>{}(std::string(#field)); \
	serializer<type>::s_fields.insert(fieldID);						\
	template<>														\
	void serializeField<type,fieldID>(const type& comp)				\
	{																\
	}													




struct myStruct
{
	int field1;
};

//SERIALIZABLE(myStruct);


SERIALIZE_FIELD(myStruct, field1);