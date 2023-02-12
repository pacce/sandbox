#ifndef SANDBOX_CAMERA_HPP__
#define SANDBOX_CAMERA_HPP__

#include <cstdint>
#include <type_traits>

#include "sandbox-point.hpp"
#include "sandbox-ray.hpp"
#include "sandbox-resolution.hpp"

namespace sandbox {
    template <typename Precision>
    class Camera {
        public:
            Camera(Resolution resolution) : Camera(Aspect<Precision>(resolution)) {}
            Camera(const Aspect<Precision>& aspect) {
                Precision vh    = 2.0;              // viewport height
                Precision vw    = aspect.width(vh); // viewport width

                origin_     = Point<Precision>::zero();
                horizontal_ = Point<Precision>( vw, 0.0, 0.0);
                vertical_   = Point<Precision>(0.0,  vh, 0.0);
                corner_     = origin_ - (horizontal_ + vertical_) * 0.5 - Point<Precision>(0.0, 0.0, 1.0);
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
