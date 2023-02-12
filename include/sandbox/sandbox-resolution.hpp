#ifndef SANDBOX_RESOLUTION_HPP__
#define SANDBOX_RESOLUTION_HPP__

#include <cstdint>
#include <cstdlib>
#include <type_traits>

#include "sandbox-random.hpp"

namespace sandbox {
    enum class Resolution {
          r2560x1080
        , r1920x1080
        , r1600x900
        , r1280x720
        , r1024x768
        , r800x600
        , r640x480
    };

namespace resolution {
    inline std::size_t
    width(Resolution resolution) {
        switch(resolution) {
            case Resolution::r2560x1080 : { return 2560; }
            case Resolution::r1920x1080 : { return 1920; }
            case Resolution::r1600x900  : { return 1600; }
            case Resolution::r1280x720  : { return 1280; }
            case Resolution::r1024x768  : { return 1024; }
            case Resolution::r800x600   : { return  800; }
            case Resolution::r640x480   : { return  640; }
        };
        return 0;
    }

    inline std::size_t
    height(Resolution resolution) {
        switch(resolution) {
            case Resolution::r2560x1080 : { return 1080; }
            case Resolution::r1920x1080 : { return 1080; }
            case Resolution::r1600x900  : { return  900; }
            case Resolution::r1280x720  : { return  720; }
            case Resolution::r1024x768  : { return  768; }
            case Resolution::r800x600   : { return  600; }
            case Resolution::r640x480   : { return  480; }
        }
        return 0;
    }
} // namespace resolution

    template <typename Precision>
    struct Fractional {
        std::size_t ws;
        std::size_t hs;

        std::pair<Precision, Precision>
        operator()(std::size_t w, std::size_t h) const {
            Precision u = static_cast<Precision>(w) / static_cast<Precision>(ws - 1);
            Precision v = static_cast<Precision>(h) / static_cast<Precision>(hs - 1);

            return {u, v};
        }

        std::pair<Precision, Precision>
        operator()(Precision w, Precision h) const {
            Precision u = w / static_cast<Precision>(ws - 1);
            Precision v = h / static_cast<Precision>(hs - 1);

            return {u, v};
        }

        std::pair<Precision, Precision>
        operator()(std::size_t w, std::size_t h, const sandbox::Random<Precision>& random) const {
            Precision u = static_cast<Precision>(w + random()) / static_cast<Precision>(ws - 1);
            Precision v = static_cast<Precision>(h + random()) / static_cast<Precision>(hs - 1);

            return {u, v};
        }
    };

    template <typename Precision>
    class Aspect {
        static_assert(std::is_floating_point<Precision>::value);
        public:
            explicit Aspect(Resolution resolution) {
                switch(resolution) {
                    case Resolution::r2560x1080 : { ratio_ = Aspect::ratio(21, 9); break; }
                    case Resolution::r1920x1080 : { ratio_ = Aspect::ratio(16, 9); break; }
                    case Resolution::r1600x900  : { ratio_ = Aspect::ratio(16, 9); break; }
                    case Resolution::r1280x720  : { ratio_ = Aspect::ratio(16, 9); break; }
                    case Resolution::r1024x768  : { ratio_ = Aspect::ratio( 4, 3); break; }
                    case Resolution::r800x600   : { ratio_ = Aspect::ratio( 4, 3); break; }
                    case Resolution::r640x480   : { ratio_ = Aspect::ratio( 4, 3); break; }
                    default: { ratio_ = Aspect::ratio(4, 3); break; }
                }
            }
            explicit Aspect(Precision ratio) : ratio_(ratio) {}
            explicit Aspect(uint8_t x, uint8_t y) {
                ratio_ = static_cast<Precision>(x) / static_cast<Precision>(y);
            }

            static Aspect fullscreen() { return Aspect(4, 3); }
            static Aspect widescreen() { return Aspect(16, 9); }

            Precision width(Precision height) const { return ratio_ * height; }
            Precision height(Precision width) const { return width / ratio_; }
        private:
            static Precision
            ratio(std::size_t x, std::size_t y) {
                return static_cast<Precision>(x) / static_cast<Precision>(y);
            }
            Precision ratio_;
    };
} // namespace sandbox

#endif // SANDBOX_RESOLUTION_HPP__
