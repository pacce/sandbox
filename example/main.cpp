#include <iostream>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 2560;

template <typename Precision>
sandbox::Color
background(const sandbox::Ray<Precision>& r) {
    sandbox::Point<Precision> p = r.direction().normalized();
    Precision t = 0.5 * (p.y() + 1.0);

    return sandbox::Color::convert(
              (1.0 - t) * 1.0 + t * 0.5
            , (1.0 - t) * 1.0 + t * 0.7
            , (1.0 - t) * 1.0 + t * 1.0
            );
}

template <typename Precision>
sandbox::Color
flat(const sandbox::Hit<Precision>& hit, const sandbox::Ray<Precision>& r) {
    if (not hit) { return background(r); } else { return sandbox::Color(255, 0, 0); }
}

template <typename Precision>
sandbox::Color
normal(const sandbox::Hit<Precision>& hit, const sandbox::Ray<Precision>& r) {
    if (not hit) {
        return background(r);
    } else {
        sandbox::Point<Precision> p = hit->normal.normalized();
        p += sandbox::Point<Precision>(1.0, 1.0, 1.0);
        p *= 0.5;

        return sandbox::Color::convert(p.x(), p.y(), p.z());
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
              sandbox::Sphere<float>(sandbox::Point<float>( 0.0f,  0.0f, -1.0f), 0.5f)
            , sandbox::Sphere<float>(sandbox::Point<float>( 1.0f,  0.0f, -1.0f), 0.5f)
            , sandbox::Sphere<float>(sandbox::Point<float>(-1.0f,  0.0f, -1.0f), 0.5f)
            , sandbox::Sphere<float>(sandbox::Point<float>( 0.0f,  1.0f, -1.0f), 0.1f)
            , sandbox::Sphere<float>(sandbox::Point<float>( 0.0f, -1.0f, -1.0f), 0.1f)
        };

        std::size_t ws  = WIDTH;
        std::size_t hs  = aspect.height(ws);

        Fractional<float> fraction = {ws, hs};

        sandbox::Colors colors = {};
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
