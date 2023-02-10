#ifndef SANDBOX_HIT_HPP__
#define SANDBOX_HIT_HPP__

#include <optional>

#include "sandbox-color.hpp"
#include "sandbox-point.hpp"
#include "sandbox-ray.hpp"

namespace sandbox {
namespace hit {
    template <typename Precision>
    struct Information {
        Precision           t;
        Point<Precision>    point;
        Point<Precision>    normal;
        Color<Precision>    color;
    };
} // namespace hit
    template <typename Precision> using Hit = std::optional<hit::Information<Precision>>;
namespace hit {
    template <typename Precision> 
    Hit<Precision>
    monad(const std::vector<Hit<Precision>>& hs) {
        Hit<Precision> value = {};
        for (const Hit<Precision>& h : hs) {
            if (not h)      { continue; }

            if (not value) { 
                value = h;
            } else {
                value = (value->t < h->t) ? value : h;
            }
        }
        return value;
    }
}
} // namespace sandbox

#endif // SANDBOX_HIT_HPP__
