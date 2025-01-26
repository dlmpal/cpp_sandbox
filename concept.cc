#include <array>
#include <concepts>
#include <format>
#include <iostream>

// Numerical concepts to be used later...
template <typename T>
concept idx_type = std::is_integral_v<T>;

template <typename T>
concept float_type = std::is_floating_point_v<T>;

template <idx_type idx_t,
          float_type float_t,
          std::size_t dim>
struct Particle
{
    idx_t id_;
    float_t mass_;
    std::array<float_t, dim> x_;
    std::array<float_t, dim> u_;
};

template <idx_type idx_t,
          float_type float_t>
std::ostream &operator<<(std::ostream &stream, const Particle<idx_t, float_t, 2> &particle)
{
    auto msg = std::format("Particle {} with x=({}, {}), u=({}, {}) and m={}\n", particle.id_,
                           particle.x_[0], particle.x_[1], particle.u_[0], particle.u_[1], particle.mass_);
    stream << msg;
    return stream;
}

int main()
{
    std::cout << Particle<int, float, 2>{.id_ = 10, .mass_ = 5, .x_ = {3, 4}, .u_ = {1, 2}};
    return 0;
}