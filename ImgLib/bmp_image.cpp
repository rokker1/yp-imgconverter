#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {


// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);

    int32_t w = image.GetWidth();
    int32_t h = image.GetHeight();
    int32_t stride = GetBMPStride(w);
    BitmapFileHeader file_header;
    file_header.file_size = stride * h;

    out.write(reinterpret_cast<const char*>(&(file_header.signature[0])), 2);
    out.write(reinterpret_cast<char*>(&(file_header.file_size)), sizeof (file_header.file_size));
    out.write(reinterpret_cast<const char*>(&(file_header.reserved_space)), sizeof (file_header.reserved_space));
    out.write(reinterpret_cast<const char*>(&(file_header.file_offset)), sizeof (file_header.file_offset));

    BitmapInfoHeader info_header;
    info_header.image_width = w;
    info_header.image_height = h;
    info_header.image_size = file_header.file_size;


    out.write(reinterpret_cast<const char*>(&(info_header.info_header_size)), sizeof(info_header.info_header_size));
    out.write(reinterpret_cast<char*>(&(info_header.image_width)), sizeof(info_header.image_width));
    out.write(reinterpret_cast<char*>(&(info_header.image_height)), sizeof(info_header.image_height));
    out.write(reinterpret_cast<const char*>(&(info_header.planes_count)), sizeof(info_header.planes_count));
    out.write(reinterpret_cast<const char*>(&(info_header.bit_per_pixel)), sizeof(info_header.bit_per_pixel));
    out.write(reinterpret_cast<const char*>(&(info_header.compression)), sizeof(info_header.compression));
    out.write(reinterpret_cast<const char*>(&(info_header.image_size)), sizeof(info_header.image_size));
    out.write(reinterpret_cast<const char*>(&(info_header.x_ppm)), sizeof(info_header.x_ppm));
    out.write(reinterpret_cast<const char*>(&(info_header.y_ppm)), sizeof(info_header.y_ppm));
    out.write(reinterpret_cast<const char*>(&(info_header.colors_in_color_table)), sizeof(info_header.colors_in_color_table));
    out.write(reinterpret_cast<const char*>(&(info_header.important_color_count)), sizeof(info_header.important_color_count));

    std::vector<char> buff (stride);
    for(int y = h - 1; y > -1; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        out.write(buff.data(), w * 3);
    }

    return out.good();
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    return {};
}

}  // namespace img_lib