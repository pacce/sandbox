#include <iostream>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 800;

template <typename Precision>
sandbox::Color
color(const sandbox::Ray<Precision>& r) {
    sandbox::Point<Precision> p = r.direction().normalized();
    Precision t = 0.5 * (p.y() + 1.0);

    return sandbox::Color::convert(
              (1.0 - t) * 1.0 + t * 0.5
            , (1.0 - t) * 1.0 + t * 0.7
            , (1.0 - t) * 1.0 + t * 1.0
            );
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
main(int argc, char** argv) {
    try {
        sandbox::Aspect aspect  = sandbox::Aspect<float>::fullscreen();
        sandbox::Camera camera  = sandbox::Camera<float>(aspect);
        sandbox::Sphere<float> sphere(sandbox::Point<float>(0.0f, 0.0f, -1.0f), 0.5f);

        std::size_t ws  = WIDTH;
        std::size_t hs  = aspect.height(ws);

        Fractional<float> fraction = {ws, hs};

        sandbox::Colors colors = {};
        colors.reserve(ws * hs);
        for (std::size_t h = 0; h < hs; h++) {
        for (std::size_t w = 0; w < ws; w++) {
            sandbox::Ray ray = camera.ray(fraction(w, h));
            if (not sphere.hit(ray)) {
                colors.push_back(color(ray));
            } else {
                colors.push_back(sandbox::Color(255, 0, 0));
            }
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
