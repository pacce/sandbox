#include <iostream>
#include <random>
#include <sandbox/sandbox.hpp>

const std::size_t SAMPLES   = 10;

using Color         = sandbox::Color<float>;
using Colors        = sandbox::Colors<float>;
using Point         = sandbox::Point<float>;
using Lambertian    = sandbox::Lambertian<float>;
using Metal         = sandbox::Metal<float>;
using Sphere        = sandbox::Sphere<float>;

template <typename Precision>
sandbox::Colors<Precision>
engine(sandbox::Resolution resolution, const std::vector<sandbox::Sphere<Precision>>& spheres) {
    sandbox::Aspect<float> aspect(resolution);
    sandbox::Camera<float> camera(aspect);

    std::size_t ws  = sandbox::resolution::width(resolution);
    std::size_t hs  = sandbox::resolution::height(resolution);

    sandbox::Random<float> random;
    sandbox::Fractional<float> fraction{ws, hs};

    Colors colors(ws * hs);

    for (std::size_t h = (hs - 1); h > 0; h--) {
        for (std::size_t w = 0; w < ws; w++) {
            std::size_t index = (h * ws) + w; 

            Colors cs;
            cs.reserve(SAMPLES);
            for (std::size_t sample = 0; sample < SAMPLES; sample++) {
                sandbox::Ray ray    = camera.ray(fraction(w, h, random));
                cs.push_back(sandbox::trace(ray, spheres, 100));
            }
            colors[index] = sandbox::colors::fold(cs).gamma();
        }
    }

    return colors;
}


int
main(int, char**) {
    try {
        sandbox::Resolution resolution = sandbox::Resolution::r800x600;
        std::size_t ws  = sandbox::resolution::width(resolution);
        std::size_t hs  = sandbox::resolution::height(resolution);

        std::vector<Sphere> spheres = {
              Sphere(Point( 0.0f,    0.0f, -1.0f),   0.5f, Lambertian(Color(0.7f, 0.3f, 0.3f)))
            , Sphere(Point( 0.0f,  100.5f, -1.0f), 100.0f, Lambertian(Color(0.8f, 0.8f, 0.0f)))
            , Sphere(Point(-1.0f,    0.0f, -1.0f),   0.5f, Metal(Color(0.8f, 0.8f, 0.8f)))
            , Sphere(Point( 1.0f,    0.0f, -1.0f),   0.5f, Metal(Color(0.8f, 0.6f, 0.2f)))
        };
        sandbox::Colors<float> colors = engine(resolution, spheres);
        sandbox::png::write("hello.png", colors, ws, hs);

        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
