/// @file image.cpp
/// Implementation of image rendering and loading

#include <cmath>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "FEHLCD.h"

#include "image.h"
#include "ui.h"
#include "util.h"

Image::Image(std::string filename) {
    stbi_uc* image = stbi_load(filename.c_str(), &w, &h, &channelCount, 4);
    colors = std::make_unique<std::vector<std::vector<uint32_t>>>(
        w, std::vector<uint32_t>(h));

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            uint32_t bytePerPixel = channelCount;
            uint8_t* pixelOffset = image + (i + w * j) * bytePerPixel;
            uint8_t r = pixelOffset[0];
            uint8_t g = pixelOffset[1];
            uint8_t b = pixelOffset[2];
            uint8_t a = channelCount >= 4 ? pixelOffset[3] : 0xff;
            colors->at(i)[j] = (a << 24) + (r << 16) + (g << 8) + (b << 0);
        }
    }

    stbi_image_free(image);
}

void Image::render(int x, int y, float theta) {
    x -= w / 2;
    y -= h / 2;

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            Vector2 node = {(float)i, (float)j};
            Vector2 center = {(float)w / 2, (float)h / 2};
            Vector2 rot = {
                center.x + (node.x - center.x) * cos(theta) -
                    (node.y - center.y) * sin(theta),
                center.y + (node.x - center.x) * sin(theta) +
                    (node.y - center.y) * cos(theta),
            };

            LCD.SetFontColor(colors->at(i)[j]);
            draw_pixel_in_bounds(x + rot.x, y + rot.y);
        }
    }
}

std::unordered_map<std::string, std::shared_ptr<Image>> ImageRepository::images;

std::shared_ptr<Image> ImageRepository::load_image(std::string filename) {
    if (!ImageRepository::images.contains(filename))
        ImageRepository::images[filename] = std::make_shared<Image>(filename);

    return ImageRepository::images[filename];
}
