#pragma once
#include <set>
#include <string>
#include <functional>
#include <map>

//@brief mark Type as Serializable. mean you builder will create a common serializer.
#define SERIALIZE

//@brief mark type file  as serializable. mean you builder will add field to common serialization
#define SERIALIZE_FIELD


constexpr std::size_t djb2Hash(std::string_view str) noexcept
{
    std::size_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // (hash * 33) + c
    }
    return hash;
}
template<class T>
struct serializer;

template<class T>
struct serializeHasher 
{
    static constexpr size_t s_hash = 0;
private:
    serializeHasher();
};
