#ifndef SANDBOX_RANDOM_HPP__
#define SANDBOX_RANDOM_HPP__

#include <cstdlib>
#include "sandbox-point.hpp"

namespace sandbox {
    template <typename Precision>
    struct Random {
        Precision
        operator()() const {
            return std::rand() / (RAND_MAX + 1.0);
        }

        Precision
        operator()(Precision minimum, Precision maximum) const {
            return minimum + (maximum - minimum) * (*this)();
        }

        Point<Precision>
        point() const {
            return Point<Precision>((*this)(), (*this)(), (*this)());
        }

        Point<Precision>
        point(Precision minimum, Precision maximum) const {
            return Point<Precision>(
                      (*this)(minimum, maximum)
                    , (*this)(minimum, maximum)
                    , (*this)(minimum, maximum)
                    );
        }

        Point<Precision>
        sphere() const {
            while(true) {
                Point<Precision> p = this->point(-1.0, 1.0);
                if (p.norm() >= 1.0) { continue; } else { return p; }
            }
        }
    };
} // namespace sandbox

#endif // SANDBOX_RANDOM_HPP__
