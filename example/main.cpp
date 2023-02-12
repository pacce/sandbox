#include <iostream>
#include <random>
#include <sandbox/sandbox.hpp>

const std::size_t SAMPLES   = 1000;

using Color         = sandbox::Color<float>;
using Colors        = sandbox::Colors<float>;
using Point         = sandbox::Point<float>;

using Lambertian    = sandbox::Lambertian<float>;
using Metal         = sandbox::Metal<float>;
using Brushed       = sandbox::Brushed<float>;
using Light         = sandbox::Light<float>;

using Sphere        = sandbox::Sphere<float>;

int
main(int, char**) {
    try {
        std::vector<Sphere> spheres = {
              Sphere(Point( 0.0f,    0.0f, -1.0f),   0.5f, Lambertian(Color(0.7f, 0.3f, 0.3f)))
            , Sphere(Point( 0.0f,    0.0f,  2.0f),   1.5f, Light(Color(1.0f, 1.0f, 1.0f)))
            , Sphere(Point(-2.0f,    0.0f,  3.0f),   0.2f, Light(Color(0.6f, 0.0f, 0.0f)))
            , Sphere(Point(-2.0f,    0.0f,  0.0f),   0.2f, Light(Color(0.0f, 0.6f, 0.0f)))
            , Sphere(Point( 0.0f, -100.5f, -1.0f), 100.0f, Lambertian(Color(0.8f, 0.8f, 0.0f)))
            , Sphere(Point(-1.0f,    0.0f, -1.0f),   0.5f, Brushed(Color(0.8f, 0.8f, 0.8f), 0.3))
            , Sphere(Point( 1.0f,    0.0f, -1.0f),   0.5f, Brushed(Color(0.8f, 0.6f, 0.2f), 1.0))
        };

        sandbox::Resolution resolution = sandbox::Resolution::r800x600;
        sandbox::Camera<float> camera(
                  Point(-3.0,  3.0,  1.0)
                , Point( 0.0,  0.0,  1.0)
                , Point( 0.0, -1.0,  0.0)
                , sandbox::FoV<float>::degrees(90.0f)
                , sandbox::Aspect<float>(resolution)
                );
        sandbox::Engine<float> engine(resolution, camera, SAMPLES);
        sandbox::Colors<float> colors = engine(spheres);
        sandbox::png::write("hello.png", colors, engine.width(), engine.height());

        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
