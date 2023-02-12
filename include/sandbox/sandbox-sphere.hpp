#ifndef SANDBOX_SPHERE_HPP__
#define SANDBOX_SPHERE_HPP__

#include "sandbox-hit.hpp"
#include "sandbox-material.hpp"
#include "sandbox-point.hpp"
#include "sandbox-ray.hpp"

namespace sandbox {
    template <typename Precision>
    class Sphere {
        public:
            Sphere(
                      const Point<Precision>&       origin
                    , Precision                     radius
                    , const Material<Precision>&    material
                    )
                : origin_(origin)
                , radius_(radius)
                , material_(material)
            {}

            const Point<Precision> origin() const   { return origin_; }
            const Precision radius() const          { return radius_; }

            Point<Precision> origin()   { return origin_; }
            Precision radius()          { return radius_; }

            Hit<Precision>
            hit(const Ray<Precision>& ray) const {
                Point<Precision> p = ray.origin() - origin_;

                Precision a = ray.direction().dot(ray.direction());
                Precision b = 2.0 * p.dot(ray.direction());
                Precision c = p.dot(p) - radius_ * radius_;

                Precision discriminant = b * b - 4.0 * a * c;
                if (discriminant > 0.0) {
                    Precision t         = (-b - std::sqrt(discriminant)) / (2.0 * a);
                    if (t < 0.0) { return {}; }

                    Point<Precision> p  = ray(t);
                    Point<Precision> n  = p - origin_;
                    n.normalize();

                    bool frontal = (ray.direction().dot(n) > 0.0);
                    return hit::Information<Precision>(
                              t                 // t
                            , p                 // point
                            , n                 // normal
                            , frontal           // ray is outside the sphere
                            , this->color()     // color
                            , this->material_   // material
                            );
                } else {
                    return {};
                }
            }
        private:
            Color<Precision>
            color() const { return std::visit(albedo_, material_); }
            Point<Precision>        origin_;
            Precision               radius_;

            Material<Precision>                     material_;
            material::visitor::Albedo<Precision>    albedo_;
    };
} // namespace sandbox

#endif // SANDBOX_SPHERE_HPP__
