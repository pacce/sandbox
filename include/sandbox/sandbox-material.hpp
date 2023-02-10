#ifndef SANDBOX_MATERIAL_HPP__
#define SANDBOX_MATERIAL_HPP__

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

            Ray<Precision>
            operator()(const hit::Information<Precision>& information) const {
                Point<Precision> p = information.normal + random_.sphere().normalized();
                return Ray<Precision>(information.point, p);
            }

            const Color<Precision>& albedo() const  { return albedo_; }
            Color<Precision>& albedo()              { return albedo_; }
        private:
            Random<Precision> random_;
            Color<Precision> albedo_;
    };
} // namespace sandbox

#endif // SANDBOX_MATERIAL_HPP__
