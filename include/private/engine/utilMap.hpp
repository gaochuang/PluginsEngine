#ifndef UTILMAP_HPP
#define UTILMAP_HPP

namespace commonApi
{

template <typename KeyType, typename ValueType>
class Map
{
public:
    using key_type = typename std::map<KeyType, ValueType>::key_type;
    using mapped_type = typename std::map<KeyType, ValueType>::mapped_type;
    using iterator = typename std::map<KeyType, ValueType>::iterator;

    Map(){}
    ~Map(){}

    void add(const key_type& key, const ValueType& value);

    ValueType get(const key_type& key) const;
    iterator begin() { return map.begin(); }
    iterator end() { return map.end(); }

    Map(const Map&) = delete;
    Map(Map&&) = delete;
    Map& operator=(const Map&) = delete;
    Map& operator=(Map&&) = delete;
private:
    std::map<KeyType, ValueType> map;
};

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::add(const key_type& key, const ValueType& value)
{
    const auto ret(map.insert(std::make_pair(key, value)));
    if (!ret.second)
    {
        ret.first->second = value;
    }
           
}

template <typename KeyType, typename ValueType>
ValueType Map<KeyType, ValueType>::get(const key_type& key) const
{
    const auto it(map.find(key));
    if (it == map.end())
    {
        return ValueType();
    }
           
    return it->second;
}

}


#endif
