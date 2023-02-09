#ifndef SANDBOX_HIT_HPP__
#define SANDBOX_HIT_HPP__

#include <optional>

#include "sandbox-point.hpp"
#include "sandbox-ray.hpp"

namespace sandbox {
    template <typename Precision>
    struct Hit {
        Precision           t;
        Point<Precision>    point;
        Point<Precision>    normal;
    };

    template <typename Precision>
    class Sphere {
        public:
            Sphere(const Point<Precision>& origin, Precision radius) 
                : origin_(origin)
                , radius_(radius)
            {}

            const Point<Precision> origin() const   { return origin_; }
            const Precision radius() const          { return radius_; }

            Point<Precision> origin()   { return origin_; }
            Precision radius()          { return radius_; }

            std::optional<Hit<Precision>>
            hit(const Ray<Precision>& ray) const {
                Point<Precision> p = ray.origin() - origin_;

                Precision a = ray.direction().dot(ray.direction());
                Precision b = 2.0 * p.dot(ray.direction());
                Precision c = p.dot(p) - radius_ * radius_;

                Precision discriminant = b * b - 4.0 * a * c;
                if (discriminant > 0.0) {
                    return Hit<Precision>(0.0, Point<Precision>(), Point<Precision>());
                } else {
                    return {};
                }
            }
        private:
            Point<Precision>    origin_;
            Precision           radius_;
    };
} // namespace sandbox

#endif // SANDBOX_HIT_HPP__