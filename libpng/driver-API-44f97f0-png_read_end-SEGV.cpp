#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <png.h>
#include <cstdint>

// A helper function to read a PNG file into a vector
std::vector<uint8_t> read_png_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }

    // Get the file size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a vector
    std::vector<uint8_t> data(size);
    if (!file.read(reinterpret_cast<char*>(data.data()), size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return {};
    }

    return data;
}

bool test_png_image_read_and_write(const std::string& filename) {
    std::vector<uint8_t> png_data = read_png_file(filename);
    if (png_data.empty()) {
        std::cerr << "Failed to read PNG file" << std::endl;
        return false;
    }

    size_t size = png_data.size();

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_memory(&image, png_data.data(), size) == 0) {
        std::cerr << "Failed to begin read from memory" << std::endl;
        return false;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        std::cerr << "Failed to create read struct" << std::endl;
        png_image_free(&image);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "Failed to create info struct" << std::endl;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        return false;
    }

    // png_set_scale_16(png_ptr);

    png_read_end(png_ptr, info_ptr);

    png_color background = {0, 0, 0};
    void *buffer = malloc(PNG_IMAGE_SIZE(image));
    if (!buffer) {
        std::cerr << "Failed to allocate buffer" << std::endl;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return false;
    }

    if (png_image_finish_read(&image, &background, buffer, 0, nullptr) != 0) {
        std::cerr << "Failed to finish read" << std::endl;
        free(buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return false;
    }

    png_alloc_size_t memory_bytes = 0;
    if (png_image_write_to_memory(&image, nullptr, &memory_bytes, 0, buffer, 0, nullptr) != 0) {
        std::cerr << "Failed to write to memory" << std::endl;
        free(buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return false;
    }

    free(buffer);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_image_free(&image);

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <png_file>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    if (test_png_image_read_and_write(filename)) {
        std::cout << "Test passed" << std::endl;
        return 0;
    } else {
        std::cout << "Test failed" << std::endl;
        return 1;
    }
}