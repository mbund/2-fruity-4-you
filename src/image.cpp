/// @file image.cpp
/// Implementation of image rendering and loading

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
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

Image::Image(const unsigned char* data, size_t data_len) {
    stbi_uc* image =
        stbi_load_from_memory(data, data_len, &w, &h, &channelCount, 4);

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

void Image::render(int x, int y, float theta) const {
    x -= w / 2;
    y -= h / 2;

    // TODO: rotate using better algorithm, maybe this:
    // https://github.com/adnanlah/rotsprite-webgl/blob/master/src/utils/RotspriteAlgoJS.ts

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            uint32_t color = colors->at(i)[j];
            // dont draw transparent pixels
            if (color << 24 == 0x00)
                continue;

            Vector2 node = {(float)i, (float)j};
            Vector2 center = {(float)w / 2, (float)h / 2};
            Vector2 rot = {
                center.x + (node.x - center.x) * cos(theta) -
                    (node.y - center.y) * sin(theta),
                center.y + (node.x - center.x) * sin(theta) +
                    (node.y - center.y) * cos(theta),
            };

            LCD.SetFontColor(color);
            draw_pixel_in_bounds(x + rot.x, y + rot.y);
        }
    }
}

#include "build/assets/apple-left.png.h"
#include "build/assets/apple-right.png.h"
#include "build/assets/apple.png.h"
#include "build/assets/background-menu.png.h"
#include "build/assets/bananas-left.png.h"
#include "build/assets/bananas-right.png.h"
#include "build/assets/bananas.png.h"
#include "build/assets/bomb.png.h"
#include "build/assets/cherries-left.png.h"
#include "build/assets/cherries-right.png.h"
#include "build/assets/cherries.png.h"
#include "build/assets/orange-left.png.h"
#include "build/assets/orange-right.png.h"
#include "build/assets/orange.png.h"
#include "build/assets/pineapple-left.png.h"
#include "build/assets/pineapple-right.png.h"
#include "build/assets/pineapple.png.h"
#include "build/assets/strawberry-left.png.h"
#include "build/assets/strawberry-right.png.h"
#include "build/assets/strawberry.png.h"

ImageRepository::ImageRepository() {
    images["assets/apple.png"] =
        std::make_shared<Image>(assets_apple_png, assets_apple_png_len);
    images["assets/apple-left.png"] = std::make_shared<Image>(
        assets_apple_left_png, assets_apple_left_png_len);
    images["assets/apple-right.png"] = std::make_shared<Image>(
        assets_apple_right_png, assets_apple_right_png_len);

    images["assets/bananas.png"] =
        std::make_shared<Image>(assets_bananas_png, assets_bananas_png_len);
    images["assets/bananas-left.png"] = std::make_shared<Image>(
        assets_bananas_left_png, assets_bananas_left_png_len);
    images["assets/bananas-right.png"] = std::make_shared<Image>(
        assets_bananas_right_png, assets_bananas_right_png_len);

    images["assets/orange.png"] =
        std::make_shared<Image>(assets_orange_png, assets_orange_png_len);
    images["assets/orange-left.png"] = std::make_shared<Image>(
        assets_orange_left_png, assets_orange_left_png_len);
    images["assets/orange-right.png"] = std::make_shared<Image>(
        assets_orange_right_png, assets_orange_right_png_len);

    images["assets/cherries.png"] =
        std::make_shared<Image>(assets_cherries_png, assets_cherries_png_len);
    images["assets/cherries-left.png"] = std::make_shared<Image>(
        assets_cherries_left_png, assets_cherries_left_png_len);
    images["assets/cherries-right.png"] = std::make_shared<Image>(
        assets_cherries_right_png, assets_cherries_right_png_len);

    images["assets/strawberry.png"] = std::make_shared<Image>(
        assets_strawberry_png, assets_strawberry_png_len);
    images["assets/strawberry-left.png"] = std::make_shared<Image>(
        assets_strawberry_left_png, assets_strawberry_left_png_len);
    images["assets/strawberry-right.png"] = std::make_shared<Image>(
        assets_strawberry_right_png, assets_strawberry_right_png_len);

    images["assets/pineapple.png"] =
        std::make_shared<Image>(assets_pineapple_png, assets_pineapple_png_len);
    images["assets/pineapple-left.png"] = std::make_shared<Image>(
        assets_pineapple_left_png, assets_pineapple_left_png_len);
    images["assets/pineapple-right.png"] = std::make_shared<Image>(
        assets_pineapple_right_png, assets_pineapple_right_png_len);

    images["assets/bomb.png"] =
        std::make_shared<Image>(assets_bomb_png, assets_bomb_png_len);

    images["assets/background-menu.png"] = std::make_shared<Image>(
        assets_background_menu_png, assets_background_menu_png_len);
}

std::shared_ptr<Image> ImageRepository::load_image(std::string filename) {
    if (images.count(filename) == 0)
        images[filename] = std::make_shared<Image>(filename);

    return images[filename];
}
