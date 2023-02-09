#ifndef SANDBOX_RAY_HPP__
#define SANDBOX_RAY_HPP__

#include "sandbox-point.hpp"

namespace sandbox {
    template <typename Precision>
    class Ray {
        public:
            Ray(const Point<Precision>& origin, const Point<Precision>& direction)
                : origin_(origin)
                , direction_(direction)
            {}

            const Point<Precision>& origin() const      { return origin_; }
            const Point<Precision>& direction() const   { return direction_; }

            Point<Precision>& origin()      { return origin_; }
            Point<Precision>& direction()   { return direction_; }

            Point<Precision>
            operator()(Precision t) const {
                return origin_ + t * direction_;
            }
        private:
            Point<Precision> origin_;
            Point<Precision> direction_;
    };
} // namespace sandbox

#endif // SANDBOX_RAY_HPP__
