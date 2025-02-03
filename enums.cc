#include <cassert>
#include <iostream>

// Unscoped enumeration
// By default, the underlying type is typically int
// Implicitly casted to its underlying type
// Members can be accessed in the enclosing scope directly
// Syntax: enum (name) (: type) {enumerator=constant-expression, ...};
enum
{
    red,
    green,
    blue
};

// Scoped enumeration
// Enumerators are not implicitly converted
// Enumerators must be accessed via name::
// Synteax: enum class (: type) {enumerator=constant-expression, ...};
enum class Letters : char
{
    a = 'a',
    b = 'b',
    c = 'c'
};

int main()
{
    // Implicitly casted to int
    assert(red + green + blue == 3);

    // std::cout << Letters::a << "\n"; Error
    std::cout << static_cast<char>(Letters::a) << "\n";

    return 0;
}