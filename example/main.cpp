#include <iostream>
#include <numeric>
#include <random>
#include <sandbox/sandbox.hpp>
#include <sstream>

const std::size_t SAMPLES   = 350;

using Color         = sandbox::Color<float>;
using Colors        = sandbox::Colors<float>;
using Point         = sandbox::Point<float>;

using Lambertian    = sandbox::Lambertian<float>;
using Metal         = sandbox::Metal<float>;
using Brushed       = sandbox::Brushed<float>;
using Light         = sandbox::Light<float>;

using Sphere        = sandbox::Sphere<float>;

template <typename Precision>
std::vector<Precision>
linspace(Precision start, Precision finish, std::size_t steps) {
    if (steps == 0) {
        return {};
    } else if (steps == 1) {
        return {start};
    } else {
        std::vector<Precision> xs;
        xs.reserve(steps);

        Precision distance = (finish - start) / static_cast<Precision>(steps - 1);
        for (std::size_t i = 0; i < steps - 1; i++) {
            xs.push_back(start + static_cast<Precision>(i) * distance);
        }
        xs.push_back(finish);
        return xs;
    }
    return {};
}

template <typename Precision>
std::vector<sandbox::Point<Precision>>
polar(Precision radius, Precision z, std::size_t steps) {
    Precision pi                = std::numbers::pi_v<Precision>;
    std::vector<Precision> vs   = linspace<Precision>(0.0, 2.0 * pi, steps);

    std::vector<sandbox::Point<Precision>> ps;
    ps.reserve(vs.size());
    for (Precision v : vs) {
        ps.emplace_back(radius * std::cos(v), radius * std::sin(v), z);
    }
    return ps;
}

inline std::string
name(std::size_t i) {
    std::stringstream ss;
    ss << "bigger-" << i << ".png";

    return ss.str();
}

int
main(int, char**) {
    try {
        std::vector<Sphere> spheres = {
              Sphere(Point( 0.0f,  0.0f,    0.0f),   0.5f, Light(Color(1.0f, 1.0f, 1.0f)))
            , Sphere(Point( 1.5f,  0.0f,    0.0f),   0.5f, Lambertian(Color(0.7f, 0.3f, 0.3f)))
            , Sphere(Point( 0.0f,  0.0f,  100.5f), 100.0f, Lambertian(Color(0.1f, 0.1f, 0.1f)))
            , Sphere(Point( 1.5f,  1.0f,    0.0f),   0.5f, Metal(Color(0.8f, 0.8f, 0.8f)))
            , Sphere(Point( 1.5f, -1.0f,    0.0f),   0.5f, Brushed(Color(0.8f, 0.6f, 0.2f), 1.0))
        };
        Point z     = Point::zaxis();
        Point at    = spheres.front().origin();

        std::vector<sandbox::Point<float>> ps = polar<float>(3.0, -5.0, 50);

        for (std::size_t i = 0; i < ps.size(); i++) {
            Point from      = ps[i];
            Point direction = (at - from).normalized();
            Point up        = z - (z.dot(direction) * direction);

            sandbox::Resolution resolution = sandbox::Resolution::r1024x768;
            sandbox::Camera<float> camera(
                      from
                    , at
                    , up.normalized()
                    , sandbox::FoV<float>::degrees(90.0f)
                    , sandbox::Aspect<float>(resolution)
                    );
            sandbox::Engine<float> engine(resolution, camera, SAMPLES);
            sandbox::Colors<float> colors = engine(spheres);
            sandbox::png::write(name(i), colors, engine.width(), engine.height());
        }

        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
