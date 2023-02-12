#ifndef SANDBOX_MATERIAL_HPP__
#define SANDBOX_MATERIAL_HPP__

#include <iostream>
#include <optional>
#include <variant>

#include "sandbox-color.hpp"
#include "sandbox-hit.hpp"
#include "sandbox-point.hpp"
#include "sandbox-random.hpp"
#include "sandbox-ray.hpp"

namespace sandbox {
    template <typename Precision>
    class Lambertian {
        public:
            Lambertian(const Color<Precision>& albedo) : albedo_(albedo) {}

            std::optional<Ray<Precision>>
            operator()(const Ray<Precision>&, const hit::Information<Precision>& information) const {
                Point<Precision> p = information.normal + random_.sphere().normalized();
                return Ray<Precision>(information.point, p);
            }

            const Color<Precision>& albedo() const  { return albedo_; }
            Color<Precision>& albedo()              { return albedo_; }
        private:
            Random<Precision> random_;
            Color<Precision> albedo_;
    };

    template <typename Precision>
    class Metal {
        public:
            Metal(const Color<Precision>& albedo) : albedo_(albedo) {}

            std::optional<Ray<Precision>>
            operator()(const Ray<Precision>& incident, const hit::Information<Precision>& information) const {
                Point<Precision> p = Metal::reflect(incident.direction().normalized(), information.normal);
                if (p.dot(information.normal) > 0) {
                    return Ray<Precision>(information.point, p);
                } else {
                    return {};
                }
            }

            const Color<Precision>& albedo() const  { return albedo_; }
            Color<Precision>& albedo()              { return albedo_; }
        private:
            static Point<Precision>
            reflect(const Point<Precision>& v, const Point<Precision>& n){
                return v - 2.0 * v.dot(n) * n;
            }
            Random<Precision> random_;
            Color<Precision> albedo_;
    };

    template <typename Precision>
    class Brushed {
        public:
            Brushed(const Color<Precision>& albedo, Precision perturbation) 
                : albedo_(albedo) 
                , perturbation_(perturbation)
            {}

            std::optional<Ray<Precision>>
            operator()(const Ray<Precision>& incident, const hit::Information<Precision>& information) const {
                Point<Precision> p = Brushed::reflect(incident.direction().normalized(), information.normal);
                p += (perturbation_ * random_.sphere());

                if (p.dot(information.normal) > 0) {
                    return Ray<Precision>(information.point, p);
                } else {
                    return {};
                }
            }

            const Color<Precision>& albedo() const  { return albedo_; }
            Color<Precision>& albedo()              { return albedo_; }
        private:
            static Point<Precision>
            reflect(const Point<Precision>& v, const Point<Precision>& n){
                return v - 2.0 * v.dot(n) * n;
            }
            Random<Precision>   random_;
            Color<Precision>    albedo_;
            Precision           perturbation_;
    };
namespace material {
namespace visitor {
    template <typename Precision>
    class Albedo {
        public:
            Color<Precision> operator()(const Lambertian<Precision>& v) const   { return v.albedo(); }
            Color<Precision> operator()(const Metal<Precision>& v) const        { return v.albedo(); }
            Color<Precision> operator()(const Brushed<Precision>& v) const      { return v.albedo(); }
        private:
    };

    template <typename Precision>
    class Scatter {
        public:
            Scatter(
                      const Ray<Precision>&                 incident
                    , const hit::Information<Precision>&    information
                    ) 
                : incident_(incident)
                , information_(information) {
                }

            ray::Scattered<Precision> operator()(const Lambertian<Precision>& v) const  { return v(incident_, information_); }
            ray::Scattered<Precision> operator()(const Metal<Precision>& v) const       { return v(incident_, information_); }
            ray::Scattered<Precision> operator()(const Brushed<Precision>& v) const     { return v(incident_, information_); }
        private:
            Ray<Precision>              incident_;
            hit::Information<Precision> information_;
    };
} // namespace visitor
    template <typename Precision>
    ray::Scattered<Precision>
    scatter(const Ray<Precision>& incident, const hit::Information<Precision>& information) {
        visitor::Scatter<Precision> visitor(incident, information);
        return std::visit(visitor, information.material);
    }
} // namespace material
} // namespace sandbox

#endif // SANDBOX_MATERIAL_HPP__
