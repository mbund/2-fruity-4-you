#pragma once

/// @file image.h
/// Image rendering and loading

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/// Image drawing
class Image {
   public:
    /// Default constructor
    /// @param filename path to image file (.png, .jpeg, etc.) to load
    Image(std::string filename);

    /// Render the image to the screen
    /// @param x x coordinate to draw the image at
    /// @param y y coordinate to draw the image at
    /// @param theta angle in radians to rotate about the center of image
    void render(int x, int y, float theta);

   private:
    int w, h, channelCount;
    std::unique_ptr<std::vector<std::vector<uint32_t>>> colors;
};

/// More optimized image storage and loader
class ImageRepository {
   public:
    /// Load an image from the path
    /// @param filename path to image file (.png, .jpeg, etc.) to load
    static std::shared_ptr<Image> load_image(std::string filename);

   private:
    static std::unordered_map<std::string, std::shared_ptr<Image>> images;
};