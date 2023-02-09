#ifndef SANDBOX_PNG_HPP__
#define SANDBOX_PNG_HPP__

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "sandbox-color.hpp"

namespace sandbox {
namespace png {
    using Pixel     = uint8_t;
    using Image     = std::vector<Pixel>;
    using Height    = std::size_t;
    using Width     = std::size_t;

    void
    write(const std::filesystem::path& path, const Image& image, Width ws, Height hs);

    void
    write(const std::filesystem::path& path, const Colors& colors, Width ws, Height hs);
} // namespace png
} // namespace sandbox

#endif // SANDBOX_PNG_HPP__
