#include <format>
#include <concepts>
#include <iostream>

void print_type_name_and_size(const std::string &type_name, std::size_t type_size)
{
    std::cout << std::format("[Type]: {}, [Size in bytes]: {}\n", type_name, type_size);
}

template <typename T>
concept IntegralType = std::is_integral_v<T>;

template <IntegralType integral_type>
void integral_type_size(const std::string &type_name)
{
    print_type_name_and_size(type_name, sizeof(integral_type));
}

template <typename T>
concept FloatingPointType = std::is_floating_point_v<T>;

template <FloatingPointType floating_point_type>
void floating_point_type_size(const std::string &type_name)
{
    print_type_name_and_size(type_name, sizeof(floating_point_type));
}

template <typename T>
concept PointerType = std::is_pointer_v<T>;

template <PointerType pointer_type>
void pointer_type_size(const std::string &type_name)
{
    print_type_name_and_size(type_name, sizeof(pointer_type));
}

// Even an empty class (no data members or virtual table pointer)
// must have a size greater than 0, so that 2 instances do not have
// the same memory address
struct EmptyClass
{
    static void print_size() { std::cout << std::format("[Type]: EmptyClass, [Size in bytes]: {}\n", sizeof(EmptyClass)); };
};

// A class thats empty (no data members), but virtual
// will have size equal to that of a pointer
struct EmptyVirtualClass
{
    static void print_size() { std::cout << std::format("[Type]: EmptyVirtualClass, [Size in bytes]: {}\n", sizeof(EmptyVirtualClass)); };
    virtual void foo() {};
};

// TODO: Alignment

int main()
{
    // Pointer sizes
    // Pointer size is usually equal to native world length
    // On a 32-bit system, sizeof(void*) should equal 4 bytes
    // On a 64-bit system, sizeof(void*) should equal 8 bytes
    // However, not all pointer types are required to be the same size
    pointer_type_size<void *>("void*");
    pointer_type_size<char *>("char*");
    pointer_type_size<int *>("int*");
    pointer_type_size<float *>("float*");
    pointer_type_size<double *>("double");
    // pointer_type_size<nullptr_t>("nullptr_t");
    print_type_name_and_size("nullptr_t", sizeof(nullptr_t));
    std::cout << "\n";

    // Reference sizes
    // A reference has a size equal to that of the object it references
    int i;
    int &ref_i = i;
    print_type_name_and_size("Int reference", sizeof(ref_i));
    double d;
    double &ref_d = d;
    print_type_name_and_size("Double reference", sizeof(ref_d));
    std::cout << "\n";

    // Integral sizes
    integral_type_size<char>("char");
    integral_type_size<wchar_t>("wchar_t");
    integral_type_size<int>("int");
    integral_type_size<size_t>("size_t"); // Note: size_t is likely equal to native word length
    integral_type_size<long>("long");
    integral_type_size<long long>("long long");
    integral_type_size<short>("short");
    integral_type_size<unsigned int>("unsigned int"); // Note: unsigned types have the same size as signed counterparts
    integral_type_size<unsigned char>("unsigned char");
    std::cout << "\n";

    // Floating point sizes
    floating_point_type_size<float>("float");
    floating_point_type_size<double>("double");
    floating_point_type_size<long double>("long double");
    std::cout << "\n";

    // Class sizes
    EmptyClass::print_size();
    EmptyVirtualClass::print_size();
}