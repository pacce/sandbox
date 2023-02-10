#include <iostream>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 2560;

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
struct Fractional {
    std::size_t ws;
    std::size_t hs;

    std::pair<Precision, Precision>
    operator()(std::size_t w, std::size_t h) const {
        Precision u = static_cast<Precision>(w) / static_cast<Precision>(ws - 1);
        Precision v = static_cast<Precision>(h) / static_cast<Precision>(hs - 1);

        return {u, v};
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

        sandbox::Colors<float> colors = {};
        colors.reserve(ws * hs);
        for (std::size_t h = hs; h > 0 ; h--) {
        for (std::size_t w = 0 ; w < ws; w++) {
            sandbox::Ray ray        = camera.ray(fraction(w, h));
            sandbox::Hit<float> hit = sandbox::trace(ray, spheres);

            colors.push_back(normal(hit, ray));
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
