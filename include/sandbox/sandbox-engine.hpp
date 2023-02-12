#ifndef SANDBOX_ENGINE_HPP__
#define SANDBOX_ENGINE_HPP__

#include "sandbox-camera.hpp"
#include "sandbox-color.hpp"
#include "sandbox-resolution.hpp"
#include "sandbox-sphere.hpp"
#include "sandbox-trace.hpp"

namespace sandbox {
    template <typename Precision>
    class Engine {
        static_assert(std::is_floating_point<Precision>::value);
        public:
            Engine(sandbox::Resolution resolution, std::size_t samples) 
                : camera_(resolution)
                , ws_(sandbox::resolution::width(resolution))
                , hs_(sandbox::resolution::height(resolution))
                , fraction_{ws_, hs_}
                , samples_(samples)
            {}

            Colors<Precision>
            operator()(const std::vector<sandbox::Sphere<Precision>>& spheres) const {
                sandbox::Colors<Precision> colors(ws_ * hs_);

                for (std::size_t h = (hs_ - 1); h > 0; h--) {
                for (std::size_t w = 0; w < ws_; w++) {
                    std::size_t index = (h * ws_) + w; 

                    Colors<Precision> cs;
                    cs.reserve(samples_);
                    for (std::size_t sample = 0; sample < samples_; sample++) {
                        sandbox::Ray ray    = camera_.ray(fraction_(w, h, random_));
                        cs.push_back(trace(ray, spheres, 100));
                    }
                    colors[index] = colors::fold(cs).gamma();
                }
                }
                return colors;
            }

            std::size_t width() const   { return ws_; }
            std::size_t height() const  { return hs_; }
        private:
            Camera<Precision>   camera_;
            std::size_t         ws_;
            std::size_t         hs_;

            sandbox::Random<Precision>      random_;
            sandbox::Fractional<Precision>  fraction_;

            std::size_t samples_;
    };
} // namespace sandbox

#endif // SANDBOX_ENGINE_HPP__
