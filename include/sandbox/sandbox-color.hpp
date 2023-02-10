#ifndef SANDBOX_COLOR_HPP__
#define SANDBOX_COLOR_HPP__

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <type_traits>
#include <vector>

namespace sandbox {
    using Channel = uint8_t;

    template <typename Precision>
    class Color {
        static_assert(std::is_floating_point<Precision>::value);
        public:
             Color() : Color(0.0, 0.0, 0.0) {}
             Color(Precision r, Precision g, Precision b) 
                : r_(std::clamp<Precision>(r, 0.0, 1.0))
                , g_(std::clamp<Precision>(g, 0.0, 1.0))
                , b_(std::clamp<Precision>(b, 0.0, 1.0))
            {}

            friend std::ostream&
            operator<<(std::ostream& os, const Color& color) {
                os << color.r_ << ", " << color.g_ << ", " << color.b_;
                return os;
            }

            Channel red() const     { return static_cast<Channel>(r_ * 255.0f);}
            Channel green() const   { return static_cast<Channel>(g_ * 255.0f);}
            Channel blue() const    { return static_cast<Channel>(b_ * 255.0f);}
        private:
            Precision r_;
            Precision g_;
            Precision b_;
    };

    template <typename Precision>
    using Colors = std::vector<Color<Precision>>;
} // namespace sandbox

#endif // SANDBOX_COLOR_HPP__
