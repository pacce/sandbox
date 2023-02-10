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

            Channel r() const     { return static_cast<Channel>(r_ * 255.0f);}
            Channel g() const   { return static_cast<Channel>(g_ * 255.0f);}
            Channel b() const    { return static_cast<Channel>(b_ * 255.0f);}

            Color<Precision>&
            operator*=(const Precision& scale) {
                r_ = std::clamp<Precision>(r_ * scale, 0.0, 1.0);
                g_ = std::clamp<Precision>(g_ * scale, 0.0, 1.0);
                b_ = std::clamp<Precision>(b_ * scale, 0.0, 1.0);

                return *this;
            }

            friend Color<Precision>
            operator*(const Color<Precision>& c, const Precision& scale) {
                Color<Precision> xs = c;
                xs *= scale;

                return xs;
            }

            friend Color<Precision>
            operator*(const Precision& scale, const Color<Precision>& c) {
                Color<Precision> xs = c;
                xs *= scale;

                return xs;
            }

            Color<Precision>&
            operator+=(const Color<Precision>& rhs) {
                r_ = std::clamp<Precision>(r_ + rhs.r_, 0.0, 1.0);
                g_ = std::clamp<Precision>(g_ + rhs.g_, 0.0, 1.0);
                b_ = std::clamp<Precision>(b_ + rhs.b_, 0.0, 1.0);

                return *this;
            }

            friend Color<Precision>
            operator+(const Color<Precision>& lhs, const Color<Precision>& rhs) {
                Color<Precision> xs = lhs;
                xs += rhs;

                return xs;
            }

            static Color<Precision> white() { return Color<Precision>(1.0, 1.0, 1.0); }
            static Color<Precision> red()   { return Color<Precision>(1.0, 0.0, 0.0); }
            static Color<Precision> green() { return Color<Precision>(0.0, 1.0, 0.0); }
            static Color<Precision> blue()  { return Color<Precision>(0.0, 0.0, 1.0); }
            static Color<Precision> black() { return Color<Precision>(0.0, 0.0, 0.0); }
        private:
            Precision r_;
            Precision g_;
            Precision b_;
    };

    template <typename Precision>
    using Colors = std::vector<Color<Precision>>;
namespace colors {
    template <typename Precision>
    Color<Precision>
    fold(const Colors<Precision>& cs) {
        using Color = Color<Precision>;
        Precision scale = 1.0 / static_cast<Precision>(cs.size());
        Color result    = Color::black();

        for (const Color& c : cs) { result += (c * scale); }
        return result;
    }
} // namespace colors
} // namespace sandbox

#endif // SANDBOX_COLOR_HPP__
