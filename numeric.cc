// <numeric> provides the functionality required to efficiently perform various operations on collections,
// which is useful for array-like programming. Functions denoted with (p) can be performed in parallel, by
// selecting their ExecutionPolicy. Most functions/algorithms in <numeric> can be placed into three categories:
//
// 1) Reduce operations
//    General formula: res = f(src[N], src[N-1], ..., src[0])
//    Functions: accumulate, inner_product, reduce (p), transform_reduce (p)
//
// 2) Partial sum operations (similar to reduce but keep intermediates)
//    General formula: dest[i] = f(src[i], src[i-1], ..., src[0])
//    Functions: inclusive_scan (p), exclusive_scan (p), transform_inclusive_scan (p), transform_exclusive_scan (p)
//
// 3) Adjacent difference operations (operate on every pair of consecutive elements)
//   General formula: dest[i] = f(src[i], src[i-1])
//   Functions: adjacent_difference (p)

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <cassert>

int main()
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

    // Sum v1 entries
    auto sum_gauss = [](int first, int last, int step)
    { return (last - first + 1) / step * (last + first) / 2; };
    auto sum1 = std::accumulate(v1.cbegin(), v1.cend(), 0, std::plus());
    auto sum2 = std::reduce(v1.begin(), v1.end(), 0, std::plus());
    assert(sum1 == sum2);
    assert(sum1 == sum_gauss(v1[0], v1[v1.size() - 1], 1));
    std::cout << "Sum of v1: " << sum1 << "\n\n";

    // Inner product of v1 and v2
    auto prod1 = std::inner_product(v1.cbegin(), v1.cend(), v2.cbegin(), 0);
    auto prod2 = std::transform_reduce(v1.cbegin(), v1.cend(), v2.cbegin(), 0, std::plus(), std::multiplies());
    assert(prod1 == prod2);
    std::cout << "Inner-product of v1 and v2: " << prod1 << "\n\n";

    // Partial sum of v1
    std::vector<int> psum1(v1.size());
    std::vector<int> psum2(v1.size());
    std::vector<int> diff(v1.size());
    std::partial_sum(v1.cbegin(), v1.cend(), psum1.begin(), std::plus());
    std::inclusive_scan(v1.cbegin(), v1.cend(), psum2.begin(), std::plus());
    assert(std::transform_reduce(psum1.cbegin(), psum1.cend(), psum2.cbegin(), 0, std::plus(), std::minus()) == 0);
    print_vector(psum1, "Partial sum of v1");

    // Partial sum of squared entries of v1
    std::vector<int> pssum(v1.size());
    std::transform_inclusive_scan(v1.cbegin(), v1.cend(), pssum.begin(), std::plus(), [](int a)
                                  { return a * a; });
    assert(pssum[v1.size() - 1] == std::accumulate(v1.cbegin(), v1.cend(), 0, [](int s, int e)
                                                   { return s + e * e; }));

    return 0;
}