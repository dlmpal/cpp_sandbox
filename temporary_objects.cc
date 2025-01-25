// Examine why named temporaries should be avoided when possible

#include <array>
#include <map>
#include <cstdio>
#include <iostream>
#include <format>

class Moveable
{
public:
    // Constructor
    Moveable(int data) : data_(data)
    {
        n_calls_map["Constructor"]++;
        std::puts("Moveable();");
    }

    // Destructor
    ~Moveable()
    {
        n_calls_map["Destructor"]++;
        std::puts("~Moveable();");
    }

    // Copy constructor
    Moveable(const Moveable &)
    {
        n_calls_map["Copy constructor"]++;
        std::puts("Moveable(const Moveable&);");
    }

    // Move constructor
    Moveable(Moveable &&)
    {
        n_calls_map["Move constructor"]++;
        std::puts("Moveable(Moveable &&);");
    }

    // Copy assignment operator
    Moveable &operator=(const Moveable &)
    {
        n_calls_map["Copy assignment"]++;
        std::puts("Moveable& operator=(const Moveable&);");
        return *this;
    }

    // Move assignment operator
    Moveable &operator=(Moveable &&)
    {
        n_calls_map["Move assignement"]++;
        std::puts("Moveable& operator=(Moveable &&);");
        return *this;
    }

    static void reset_n_calls()
    {
        n_calls_map.clear();
    }

    static void print_n_calls()
    {
        for (const auto &[k, v] : n_calls_map)
        {
            std::cout << std::format("{}: {}\n", k, v);
        }
    }

private:
    inline static std::map<std::string, int> n_calls_map;
    int data_; // ignored
};

int main()
{
    // Create scope so that destructor calls can be observed
    {
        // Create two named temporary objects
        Moveable m1{10};
        Moveable m2{20};

        // Create an array with those two temporaries (copy)
        std::array<Moveable, 2> array{m1, m2};
        std::array<int, 3> a{1, 2, 3};
    }

    // Observe that the entries of array are copy-constructed
    Moveable::print_n_calls();
    Moveable::reset_n_calls();

    // Avoid copy-construction using std::move
    {
        // Create two named temporary objects
        Moveable m1{10};
        Moveable m2{20};

        // Create an array with those two temporaries (move)
        std::array<Moveable, 2> array{std::move(m1), std::move(m2)};
    }

    // Observe that the entries of array are now move-constructed
    Moveable::print_n_calls();
    Moveable::reset_n_calls();

    // Still, the creation of the temporary objects is unnecessary ...
    {
        // Example function that creates the objects
        auto create_moveable = [](int data)
        { return Moveable(data); };

        // Create an array by directly-initializing its entries
        // Move/Copy elision
        std::array<Moveable, 2> array{create_moveable(10), create_moveable(20)};
    }

    // Observe that the entries of array are now directly-initialized
    // No redudant function calls
    Moveable::print_n_calls();

    return 0;
}