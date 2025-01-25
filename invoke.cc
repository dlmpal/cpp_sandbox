// std::invoke (included in <functional> provides a uniform interface
// for calling functors (also SFINAE).
// Note: std::apply is similar to invoke, but accepts the arguments
// as a tuple, instead of as variadic arguments.

#include <vector>
#include <functional>
#include <cassert>

int main()
{
    std::vector<int> a(3, 10);
    auto pa = &a;
    assert(std::invoke(&std::vector<int>::size, pa) == std::invoke(&std::vector<int>::size, a));

    return 0;
}
