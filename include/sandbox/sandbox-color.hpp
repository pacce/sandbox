#ifndef SANDBOX_COLOR_HPP__
#define SANDBOX_COLOR_HPP__

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <vector>

namespace sandbox {
    using Channel = uint8_t;

    class Color {
        public:
            Color() : Color(0, 0, 0) {}
            Color(Channel r, Channel g, Channel b) : red_(r), green_(g), blue_(b) {}

            template <typename Precision>
            static Color
            convert(Precision r, Precision g, Precision b) {
                static_assert(std::is_floating_point<Precision>::value);

                r = std::clamp<Precision>(r, 0.0, 1.0) * 255.0;
                g = std::clamp<Precision>(g, 0.0, 1.0) * 255.0;
                b = std::clamp<Precision>(b, 0.0, 1.0) * 255.0;

                return Color(static_cast<Channel>(r), static_cast<Channel>(g), static_cast<Channel>(b));
            }

            friend std::ostream&
            operator<<(std::ostream& os, const Color& color) {
                os  << color.red_
                    << ", "
                    << color.green_
                    << ", "
                    << color.blue_
                    ;
                return os;
            }

            Channel red() const     { return red_;}
            Channel green() const   { return green_;}
            Channel blue() const    { return blue_;}
        private:
            Channel red_;
            Channel green_;
            Channel blue_;
    };

    using Colors = std::vector<Color>;
} // namespace sandbox

#endif // SANDBOX_COLOR_HPP__
