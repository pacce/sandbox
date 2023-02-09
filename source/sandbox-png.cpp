#include "sandbox/sandbox-png.hpp"

#include <png.h>
#include <cstdio>
#include <stdexcept>

namespace sandbox {
namespace png {
    void
    write(const std::filesystem::path&  path, const Image& image, Width ws, Height hs) {
        std::FILE * handle = std::fopen(path.c_str(), "wb");
        if (handle == nullptr) {
            throw std::runtime_error("could not open file");
        }

        png_structp png = png_create_write_struct(
                PNG_LIBPNG_VER_STRING
                , NULL
                , NULL
                , NULL
                );
        if (png == nullptr) {
            throw std::runtime_error("png_create_write_struct error");
        }

        png_infop info = png_create_info_struct(png);
        if (info == nullptr) {
            throw std::runtime_error("png_create_info_struct");
        }
        png_init_io(png, handle);
        png_set_IHDR(
                png
                , info
                , static_cast<png_uint_32>(ws)
                , static_cast<png_uint_32>(hs)
                , 8
                , PNG_COLOR_TYPE_RGBA
                , PNG_INTERLACE_NONE
                , PNG_COMPRESSION_TYPE_DEFAULT
                , PNG_FILTER_TYPE_DEFAULT
                );
        png_write_info(png, info);

        png_bytep * rows    = new png_bytep[hs];
        png_uint_32 bytes   = png_get_rowbytes(png, info);

        for (std::size_t h = 0; h < hs; h++) {
            png_byte * row  = new png_byte[bytes];
            rows[h]         = row;
            for (std::size_t w = 0; w < ws; w++) {
                std::size_t index = ((h * ws) + w) * 4;

                *row++  = image[index + 0];
                *row++  = image[index + 1];
                *row++  = image[index + 2];
                *row++  = image[index + 3];
            }
        }

        png_write_image(png, rows);
        png_write_end(png, NULL);

        for (std::size_t h = 0; h < hs; h++) {
            delete[] rows[h];
        }
        delete[] rows;

        png_destroy_write_struct(&png, &info);
        std::fclose(handle);
    }
} // namespace png
} // namespace sandbox
