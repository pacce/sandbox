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

int
main(int, char**) {
    try {
        std::vector<Sphere> spheres = {
              Sphere(Point( 0.0f,    0.0f, -1.0f),   0.5f, Lambertian(Color(0.7f, 0.3f, 0.3f)))
            , Sphere(Point( 0.0f,  100.5f, -1.0f), 100.0f, Lambertian(Color(0.8f, 0.8f, 0.0f)))
            , Sphere(Point(-1.0f,    0.0f, -1.0f),   0.5f, Metal(Color(0.8f, 0.8f, 0.8f)))
            , Sphere(Point( 1.0f,    0.0f, -1.0f),   0.5f, Metal(Color(0.8f, 0.6f, 0.2f)))
        };

        sandbox::Engine<float> engine(sandbox::Resolution::r2560x1080, SAMPLES);
        sandbox::Colors<float> colors = engine(spheres);
        sandbox::png::write("hello.png", colors, engine.width(), engine.height());

        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
