#include <iostream>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 800;
const std::size_t HEIGHT    = 600;

int
main(int argc, char** argv) {
    try {
        sandbox::Colors colors = {};
        colors.reserve(WIDTH * HEIGHT);

        for (std::size_t w = 0; w < WIDTH; w++) {
        for (std::size_t h = 0; h < HEIGHT; h++) {
            colors.emplace_back(0, 255, 0);
        }
        }
        sandbox::png::write("hello.png", colors, WIDTH, HEIGHT);
        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
