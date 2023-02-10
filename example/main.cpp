#include <iostream>
#include <random>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 1024;
const std::size_t SAMPLES   = 100;

template <typename Precision>
sandbox::Color<Precision>
background(const sandbox::Ray<Precision>& r) {
    using Point = sandbox::Point<Precision>;
    using Color = sandbox::Color<Precision>;

    Point p     = r.direction().normalized();
    Precision t = 0.5 * (p.y() + 1.0);

    return (1.0 - t) * Color::white() + t * Color(0.5, 0.7, 1.0);
}

template <typename Precision>
sandbox::Color<Precision>
flat(const sandbox::Hit<Precision>& hit, const sandbox::Ray<Precision>& r) {
    if (not hit) { return background(r); } else { return sandbox::Color<Precision>::red(); }
}

template <typename Precision>
sandbox::Color<Precision>
normal(const sandbox::Hit<Precision>& hit, const sandbox::Ray<Precision>& r) {
    if (not hit) {
        return background(r);
    } else {
        sandbox::Point<Precision> p = hit->normal.normalized();
        p += sandbox::Point<Precision>(1.0, 1.0, 1.0);
        p *= 0.5;

        return sandbox::Color<Precision>(p.x(), p.y(), p.z());
    }
}

template <typename Precision>
class Random {
    public:
        Random() : random_{}, mersenne_{random_()} , distribution_(0.0, 1.0) {}

        Precision
        operator()() { return distribution_(mersenne_); }
    private:
        std::random_device  random_;
        std::mt19937        mersenne_;

        std::uniform_real_distribution<Precision> distribution_;
};

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
    operator()(std::size_t w, std::size_t h, Random<Precision>& random) const {
        return (*this)(static_cast<Precision>(w), static_cast<Precision>(h), random);
    }

    std::pair<Precision, Precision>
    operator()(Precision w, Precision h, Random<Precision>& random) const {
        return (*this)(w + random(), h + random());
    }
};

int
main(int, char**) {
    try {
        sandbox::Aspect aspect  = sandbox::Aspect<float>::widescreen();
        sandbox::Camera camera  = sandbox::Camera<float>(aspect);
        std::vector<sandbox::Sphere<float>> spheres = {
              sandbox::Sphere<float>(sandbox::Point<float>( 0.0f,    0.0f, -1.0f),   0.5f)
            , sandbox::Sphere<float>(sandbox::Point<float>( 0.0f, -100.5f, -1.0f), 100.0f)
        };

        std::size_t ws  = WIDTH;
        std::size_t hs  = aspect.height(ws);

        Fractional<float> fraction = {ws, hs};
        Random<float> random;

        sandbox::Colors<float> colors = {};
        colors.reserve(ws * hs);
        for (std::size_t h = hs; h > 0 ; h--) {
        for (std::size_t w = 0 ; w < ws; w++) {
            sandbox::Colors<float> cs;
            cs.reserve(SAMPLES);

            for (std::size_t sample = 0; sample < SAMPLES; sample++) {
                sandbox::Ray ray        = camera.ray(fraction(w, h, random));
                sandbox::Hit<float> hit = sandbox::trace(ray, spheres);

                cs.push_back(normal(hit, ray));
            }

            colors.push_back(sandbox::colors::fold(cs));
        }
        }
        sandbox::png::write("hello.png", colors, ws, hs);
        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
