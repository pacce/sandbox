#ifndef SANDBOX_POINT_HPP__
#define SANDBOX_POINT_HPP__

#include <geometry/geometry.hpp>

namespace sandbox {
    template <typename Precision>
    using Point = geometry::d3::Point<Precision>;
}

#endif // SANDBOX_POINT_HPP__
