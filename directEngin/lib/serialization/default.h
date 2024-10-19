#ifndef __SERIALIZATION_DEFAULT__
#define __SERIALIZATION_DEFAULT__
#include "../resmngr/xml_node.h"

template<typename T>
struct Serializer
{
	static void serialize(XMLNode node, const T& value)
	{
		node.setValue<T>(value);
	}

	static T deserialize(XMLNode node)
	{
		return node.value<T>();
	}
};

//TODO implement default serializers

#endif // !__SERIALIZATION_DEFAULT__
