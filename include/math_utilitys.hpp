#pragma ONCE
#include <eigen3/Eigen/Core>
#include <cmath>
#include <ranges>

constexpr auto range_n_spaced(int resolution, double start, double end) {
    return std::views::iota(0, resolution)
         | std::views::transform([=](int i) {
               return start + i * (end - start) / (resolution - 1);
           });
}