// <algorithm> provides functionality related to the manipulation of collections

#include <array>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>

void compare_vectors()
{
    // Non-capturing lambda
    auto print_vector = [](const std::vector<int> &v, const std::string &msg)
    {std::cout << msg << "\n"; for(auto e: v) std::cout << e << " "; std::cout<<"\n\n"; };

    // Example vectors
    std::vector<int> v1(5);
    std::iota(v1.begin(), v1.end(), 1);
    print_vector(v1, "v1");

    std::vector<int> v2(5);
    std::fill(v2.begin(), v2.end(), -1);
    print_vector(v2, "v2");

    // v1 < v2 ? true : false
    std::cout << "v1 < v2: " << std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()) << "\n\n";

    // v1 == v2 ? true : false
    std::cout << "v1 == v2: " << std::equal(v1.begin(), v1.end(), v2.begin(), v2.end()) << "\n\n";
}

void search()
{
    // Find element in vector
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    int three = 3;
    auto result1 = std::find(v1.cbegin(), v1.cend(), three);
    auto result2 = std::find_if(v1.cbegin(), v1.cend(), [three](int e)
                                {if(e==three)return true; return false; });
    assert(result1 == result2);
    assert(result1 != v1.end());

    // Check that all entries of v2 = partial_sum(v1) > 1
    std::vector<int> v2(v1.size());
    std::inclusive_scan(v1.cbegin(), v1.cend(), v2.begin(), std::plus());
    assert(std::all_of(v2.cbegin(), v2.cend(), [](int e)
                       { return e >= 1; }) == true);

    // Check that array exists inside vector
    // Note: iterators should be of "same" type
    std::array<int, 3> a1{1, 2, 3};
    assert(std::search(v1.cbegin(), v1.cend(), a1.cbegin(), a1.cend()) != v1.end());

    // Check that array exists inside map
    using array_3 = std::array<int, 3>;
    auto array_equal = [&a1](auto e)
    { return std::equal(e.second.cbegin(), e.second.cend(), a1.cbegin()); };
    std::unordered_map<int, array_3> um1{{0, {1, 2, 3}},
                                         {1, {2, 3, 4}},
                                         {2, {3, 4, 5}}};
    assert(std::any_of(um1.cbegin(), um1.cend(), array_equal) == true);        // any_of returns bool
    assert(std::find_if(um1.cbegin(), um1.cend(), array_equal) != um1.cend()); // find_if returns iterator
}

void remove_erase()
{
    // What a nice lambda...
    auto print_vector = []<typename T>(const std::vector<T> &v)
    {std::for_each(v.cbegin(), v.cend(), [](auto e){std::cout << e << " ";}); std::cout << "\n"; };

    // Create two vectors of size 10
    int n = 10;
    std::vector<int> v1(n);
    std::iota(v1.begin(), v1.end(), 1);
    std::vector<int> v2;
    std::copy(v1.cbegin(), v1.cend(), std::back_inserter(v2));
    assert(std::equal(v1.cbegin(), v1.cend(), v2.cbegin()) == true);

    // Predicate
    auto is_even = []<typename T>(T e)
    { return e % 2; };

    // Remove-erase idiom
    // std::remove(_if) returns an iterator (it)
    // which points to the tail of the original vector,
    // containing junk.
    // Then, the tail is erased using v.erase(it, v.end())
    v1.erase(std::remove_if(v1.begin(), v1.end(), is_even), v1.end());

    // std::erase(_if) obviates the use of the remove-erase idiom
    std::erase_if(v2, is_even);

    // Check that the two methods produce the same vector
    assert(std::equal(v1.cbegin(), v1.cend(), v2.cbegin()));
    print_vector(v1);
    print_vector(v2);
}

void unique()
{
    // What a nice lambda...
    auto print_vector = []<typename T>(const std::vector<T> &v)
    {std::for_each(v.cbegin(), v.cend(), [](auto e){std::cout << e << " ";}); std::cout << "\n"; };

    auto is_even = []<typename T>(T e)
    { return e % 2; };

    size_t n = 20;
    std::vector<int> v1;
    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(i);
        v1.push_back(i);
    }

    print_vector(v1);
    std::cout << "No. even: " << std::count_if(v1.cbegin(), v1.cend(), is_even) << "\n";

    v1.erase(std::unique(v1.begin(), v1.end()), v1.end());
    print_vector(v1);
    std::cout << "No. even: " << std::count_if(v1.cbegin(), v1.cend(), is_even) << "\n";
}

int main()
{
    compare_vectors();
    search();
    remove_erase();
    unique();
    return 0;
}