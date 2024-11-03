#ifndef __SERIALIZATION_DEFAULT__
#define __SERIALIZATION_DEFAULT__

template<typename T>
struct Serializer
{
	static void serialize(XMLNode node, const T& value)
	{
		node.setValue(value);
	}

	static T deserialize(XMLNode node)
	{
		return node.value(T());
	}

	static void deserialize(XMLNode node, T& value)
	{
		value = node.value(T());
	}
};

#endif // !__SERIALIZATION_DEFAULT__
