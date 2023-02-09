#include <iostream>
#include <sandbox/sandbox.hpp>

const std::size_t WIDTH     = 800;
const std::size_t HEIGHT    = 600;

int
main(int argc, char** argv) {
    try {
        sandbox::png::Image image = {};
        for (std::size_t w = 0; w < WIDTH; w++) {
        for (std::size_t h = 0; h < HEIGHT; h++) {
            image.push_back(255);   // Red
            image.push_back(  0);   // Green
            image.push_back(  0);   // Blue
            image.push_back(255);   // Alpha
        }
        }
        sandbox::png::write("hello.png", image, WIDTH, HEIGHT);
        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
