#include <array>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <format>
#include <iostream>
#include <exception>

template <typename KeyType, typename ValueType, std::size_t Size>
class Map
{
public:
    Map(const std::array<std::pair<KeyType, ValueType>, Size> &data)
        : data_(data)
    {
    }

    constexpr ValueType at(const KeyType &key) const
    {
        const auto it = std::find_if(data_.cbegin(), data_.cend(),
                                     [&key](const auto &kv)
                                     { return kv.first == key; });

        if (it == data_.end())
        {
            throw(std::range_error(std::format("{} not found in map.\n", key)));
        }

        return it->second;
    }

private:
    std::array<std::pair<KeyType, ValueType>, Size> data_;
};

int main()
{
    static constexpr std::array<std::pair<std::string, int>, 3> data{{{"red", 1},
                                                                      {"blue", 2},
                                                                      {"green", 3}}};

    Map<std::string, int, 3> map(data);

    try
    {
        std::string key{"purple"};
        decltype(auto) value = map.at(key);
        std::cout << std::format("{}: {}\n", key, value);
    }
    catch (const std::range_error &e)
    {
        std::cerr << e.what();
    }

    return 0;
}