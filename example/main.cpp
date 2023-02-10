// #include <omp.h>

#include <iostream>
#include <random>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 2560;
const std::size_t SAMPLES   = 2;

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

        sandbox::Random<float> random;
        Fractional<float> fraction = {ws, hs};

        sandbox::Colors<float> colors(ws * hs);

 // #pragma omp parallel for collapse(2)
        for (std::size_t h = (hs - 1); h > 0; h--) {
        for (std::size_t w = 0; w < ws; w++) {
            std::size_t index = (h * ws) + w; 

            sandbox::Colors<float> cs;
            cs.reserve(SAMPLES);
            for (std::size_t sample = 0; sample < SAMPLES; sample++) {
                sandbox::Ray ray    = camera.ray(fraction(w, h, random));
                cs.push_back(sandbox::trace(ray, spheres, 5));
            }
            colors[index] = sandbox::colors::fold(cs).gamma();
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
