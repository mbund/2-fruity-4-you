#pragma once

/// @file image.h
/// @author Mark Bundschuh
/// @brief Image rendering and loading

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/// Render an image (.png, .jpeg, etc.)
class Image {
   public:
    /// Create and image based on a file path
    /// @param filename path to image file (.png, .jpeg, etc.) to load
    Image(std::string filename);

    /// Render the image to the screen
    /// @param x x coordinate to draw the image at
    /// @param y y coordinate to draw the image at
    /// @param theta angle in radians to rotate about the center of image
    void render(int x, int y, float theta) const;

    Image(const unsigned char* data, size_t data_len);

   private:
    int w, h, channelCount;
    std::unique_ptr<std::vector<std::vector<uint32_t>>> colors;
};

/// Optimized Image storage and loading
class ImageRepository {
   public:
    /// Default constructor
    ImageRepository();

    /// Load an image from the path
    /// @param filename path to image file (.png, .jpeg, etc.) to load
    std::shared_ptr<Image> load_image(std::string filename);

   private:
    std::unordered_map<std::string, std::shared_ptr<Image>> images;
};

/// Global variable to hold the image repository
inline auto image_repository = std::make_shared<ImageRepository>();