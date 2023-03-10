#ifndef SANDBOX_TRACE_HP__
#define SANDBOX_TRACE_HP__

#include "sandbox-color.hpp"
#include "sandbox-hit.hpp"
#include "sandbox-point.hpp"
#include "sandbox-random.hpp"
#include "sandbox-ray.hpp"
#include "sandbox-sphere.hpp"

namespace sandbox {
    template <typename Precision>
    sandbox::Color<Precision>
    background(const sandbox::Ray<Precision>& r) {
        using Point = sandbox::Point<Precision>;
        using Color = sandbox::Color<Precision>;

        Point p     = r.direction().normalized();
        Precision t = 0.5 * (p.y() + 1.0);

        return (1.0 - t) * Color::white() + t * Color(0.5, 0.7, 1.0);
    }

    template <typename Precision>
    sandbox::Color<Precision>
    flat(const sandbox::Hit<Precision>& hit, const sandbox::Ray<Precision>& r) {
        if (not hit) { return background(r); } else { return sandbox::Color<Precision>::red(); }
    }

    template <typename Precision>
    sandbox::Color<Precision>
    normal(const sandbox::Hit<Precision>& hit, const sandbox::Ray<Precision>& r) {
        if (not hit) {
            return background(r);
        } else {
            sandbox::Point<Precision> p = hit->normal.normalized();
            p += sandbox::Point<Precision>(1.0, 1.0, 1.0);
            p *= 0.5;

            return sandbox::Color<Precision>(p.x(), p.y(), p.z());
        }
    }

    template <typename Precision>
    sandbox::Color<Precision>
    trace(const Ray<Precision>& ray, const std::vector<Sphere<Precision>>& spheres, std::size_t depth = 0) {
        if (depth == 0) { return Color<Precision>::black(); }

        std::vector<Hit<Precision>> hs;
        hs.reserve(spheres.size());

        for (const Sphere<Precision>& sphere : spheres) { hs.push_back(sphere.hit(ray)); }
        Hit<Precision> hit = hit::monad(hs);

        if (not hit) {
            return 0.1 * background(ray);
        } else {
            ray::Scattered<Precision> scattered = material::scatter(ray, *hit);
            Color<Precision> emitted            = material::emitter(*hit);

            if (not scattered) {
                return emitted;
            } else {
                return emitted + hit->color * trace(*scattered, spheres, depth--);
            }
        }
    }
} // namespace sandbox

#endif // SANDBOX_TRACE_HP__
