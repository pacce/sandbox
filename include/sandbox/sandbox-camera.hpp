#ifndef SANDBOX_CAMERA_HPP__
#define SANDBOX_CAMERA_HPP__

#include <cstdint>
#include <numbers>
#include <type_traits>

#include "sandbox-point.hpp"
#include "sandbox-ray.hpp"
#include "sandbox-resolution.hpp"

namespace sandbox {
    template <typename Precision>
    class FoV {
        static_assert(std::is_floating_point<Precision>::value);
        public:
            FoV(Precision radians) : radians_(radians) {}

            static FoV
            degrees(Precision value) {
                Precision radians  = (value * std::numbers::pi_v<Precision>) / 180.0;
                return FoV(radians);
            }

            Precision
            height() const { return std::tan(radians_ / 2.0); }
        private:
            Precision radians_;
    };

    template <typename Precision>
    class Camera {
        public:
            Camera(
                      const Point<Precision>&   from
                    , const Point<Precision>&   at
                    , const Point<Precision>&   up
                    , const FoV<Precision>&     fov
                    , const Aspect<Precision>&  aspect
                    )
            {
                Precision vh    = fov.height() * 2.0f;  // viewport height
                Precision vw    = aspect.width(vh);     // viewport width

                Point<Precision> w = (from - at).normalized();
                Point<Precision> u = (up.cross(w)).normalized();
                Point<Precision> v = w.cross(u);

                origin_     = from;
                horizontal_ = vw * u;
                vertical_   = vh * v;
                corner_     = origin_ - (horizontal_ + vertical_) * 0.5 - w;
            }

            Camera() : Camera(Aspect<Precision>::widescreen()) {}

            Ray<Precision>
            ray(Precision u, Precision v) const {
                Point<Precision> direction = corner_ + u * horizontal_ + v * vertical_;

                return Ray<Precision>(origin_, direction - origin_);
            }

            Ray<Precision>
            ray(const std::pair<Precision, Precision>& uv) const {
                return this->ray(uv.first, uv.second);
            }
        private:
            Point<Precision> origin_;

            Point<Precision> horizontal_;
            Point<Precision> vertical_;

            Point<Precision> corner_;
    };
} // namespace sandbox

#endif // SANDBOX_CAMERA_HPP__
