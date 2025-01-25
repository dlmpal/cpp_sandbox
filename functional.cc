// <functional> provides various tools for functional programming in C++,
// such as partial function application via std::bind, std::mem_fn, lambdas

#include <vector>
#include <iostream>
#include <numeric>
#include <functional>
#include <cassert>

int main()
{
    // std::bind -> partial function application
    using namespace std::placeholders; // the i-th placeholder _i refers to its position in the partial function call
    auto print_x_y_z = [](int x, int y, int z)
    { std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n"; };
    auto print_1_2_3 = std::bind(print_x_y_z, 1, 2, 3);
    print_1_2_3();
    auto print_1_y_3 = std::bind(print_x_y_z, 1, _1, 3);
    print_1_y_3(0); // = print_x_y_z(1, _1=0, 3)
    auto print_z_y_x = std::bind(print_x_y_z, _3, _2, _1);
    print_z_y_x(1, 2, 3); // = print_z_y_x(_1, _2, _3) = print_x_y_z(_3=3, _2=2, _1=1)

    // lambdas can also be used for partial function application
    // and perhaps more conveniently
    auto print_4_5_6 = [&print_x_y_z]()
    { print_x_y_z(4, 5, 6); };
    print_4_5_6();

    // std::mem_fn -> stores a pointer to a member
    // similar to std::bind, but should be preferred for member funcs
    std::vector<int> v1(5);
    std::iota(v1.begin(), v1.end(), 1);
    auto vector_size_1 = std::mem_fn(&std::vector<int>::size);
    auto vector_size_2 = std::bind(&std::vector<int>::size, _1);
    assert(vector_size_1(v1) == vector_size_2(v1));
    assert(vector_size_1(v1) == v1.size());

    return 0;
}